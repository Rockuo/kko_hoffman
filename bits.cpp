/**
 * Richard Bureš (xbures29)
 * implementace bit stream
 */


#include <string>
#include <vector>
#include "bits.h"
#include "errors.h"

/**
 * Určí, jestli se čtecí hlava dostala na konec bit streamu
 * @param bits
 * @return
 */
bool bitsEnd(Bits *bits) {
    return (bits->mainHead == (bits->parts.size() - 1) && bits->partHead == ((bits->tail - 1)%8)) ||
            (bits->mainHead == (bits->parts.size())) // failsafe
            ;
}


/**
 * Přečte bit ze streamu
 * @param bits
 * @return
 */
bool readBit(Bits *bits) {
    unsigned char part = (bits->parts)[bits->mainHead];
    // přečte bit
    bool result = ((part >> bits->partHead) & 1u) == 1u;

    // posune čtecí hlavu
    if (bits->partHead == 0u) {
        bits->partHead = 7u;
        bits->mainHead++;
    } else {
        bits->partHead--;
    }
    return result;
}

/**
 * Uloží bit stream do řetězce
 * @param bits
 * @return
 */
std::string saveBits(Bits *bits) {
    // uloží informaci kolik bitů z streamu je navíc pro započtení na Byty
    if (((bits->tail >> 3u) & 1u) == 1u) {
        bits->parts[0] = (1u << 7u) | (bits->parts[0]);
    }
    if (((bits->tail >> 2u) & 1u) == 1u) {
        bits->parts[0] = (1u << 6u) | (bits->parts[0]);
    }
    if (((bits->tail >> 1u) & 1u) == 1u) {
        bits->parts[0] = (1u << 5u) | (bits->parts[0]);
    }
    if (((bits->tail >> 0u) & 1u) == 1u) {
        bits->parts[0] = (1u << 4u) | (bits->parts[0]);
    }
    // uloží bit stream
    return std::string(bits->parts.begin(), bits->parts.end());
}

/**
 * Resetuje čtecí hlavu/y
 * @param bits
 */
void resetHeads(Bits *bits) {
    bits->partHead = 7u;
    bits->mainHead = 0u;
}

/**
 * Převede string na bit stream (zkontroluje, že se jedná o správný stream -> použití adaptive/static, použité modelu)
 * @param str
 * @param adaptive
 * @param model
 * @return
 */
Bits *loadBits(std::string *str, bool adaptive, bool model) {
    // převod
    Bits *bits = new Bits;
    resetHeads(bits);
    std::copy((*str).begin(), (*str).end(), std::back_inserter(bits->parts));

    // načtení kolik bytů je doplnění do Bytu
    if (readBit(bits)) {
        bits->tail = (1u << 3u) | (bits->tail);
    }
    if (readBit(bits)) {
        bits->tail = (1u << 2u) | (bits->tail);
    }
    if (readBit(bits)) {
        bits->tail = (1u << 1u) | (bits->tail);
    }
    if (readBit(bits)) {
        bits->tail = (1u << 0u) | (bits->tail);
    }

    // kontrola parametru adaptive
    if(readBit(bits) != adaptive)
    {
        throw  InvalidAdaptiveOption();
    }
    // kontrola modelu
    if(readBit(bits) != model)
    {
        throw InvalidModelOption();
    }

    return bits;
}

/**
 * Uží jeden bit do bit streamu
 * @param bits
 * @param bit
 */
void pushBit(Bits *bits, bool bit) {
    // init hlavy
    if (bits->partHead == 255u) {
        bits->parts.push_back(0u);
        bits->partHead = 7u;
        bits->tail = 8u;
        bits->mainHead++;
    }
    // pokud true, uloží 1, jinak tam už 0 je
    if (bit) {
        bits->parts[bits->parts.size() - 1] = (1u << bits->partHead) | (bits->parts.back());
    }

    // posun hlavy
    bits->partHead--;
    bits->tail--;
}

/**
 * Inicializuej bit stream
 * @param bits
 */
Bits *createBits(bool adaptive, bool model) {
    Bits *bits = new Bits;
    // alokace místa pro uložení tail (kolik bytu doplnuje do Bytu)
    pushBit(bits, false);
    pushBit(bits, false);
    pushBit(bits, false);
    pushBit(bits, false);

    //  uložení modelu a adaptivnosti
    pushBit(bits, adaptive);
    pushBit(bits, model);

    // návrat hlavy na init hodnotu
    bits->mainHead--;
    return bits;
}


/**
 * Uloží číslodo bit streamu
 * @param bits
 * @param number
 */
void pushNumber(Bits *bits, unsigned int number) {
    for (unsigned char i = 8u; i > 0u; i--) {
        pushBit(bits, ((number >> (i - 1u)) & 1u) == 1u);
    }
}

/**
 * Přečte číslo z bit streamu
 * @param bits
 * @return
 */
unsigned char readNumber(Bits *bits) {
    unsigned char result = 0u;
    for (unsigned char i = 8u; i > 0u; i--) {
        if (readBit(bits)) {
            result = (1u << (i - 1u)) | result;
        }
    }
    return result;
}


/**
 * Uloží kod charu
 * @param bits
 * @param code
 */
void pushCode(Bits *bits, std::vector<bool> *code) {
    for (bool b: (*code)) {
        pushBit(bits, b);
    }
}
