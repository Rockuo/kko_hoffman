/**
 * Richard Bureš (xbures29)
 * Bit stream interface
 */

#include <vector>
#include <string>

#ifndef PROJEKT_BITS_H
#define PROJEKT_BITS_H

/**
 * Struktura bit streamu
 */
struct Bits {

    std::vector<unsigned char> parts;
    unsigned char partHead = 255u; // 0->7 (255 značí přetečení do další části)
    unsigned char tail = 0u; // 0->8
    unsigned long int mainHead = 0u; // 0->partHead.size()-1
};


/**
 * Určí, jestli se čtecí hlava dostala na konec bit streamu
 * @param bits
 * @return
 */
bool bitsEnd(Bits *bits);

/**
 * Přečte bit ze streamu
 * @param bits
 * @return
 */
bool readBit(Bits *bits);

/**
 * Uloží bit stream do řetězce
 * @param bits
 * @return
 */
std::string saveBits(Bits *bits);


/**
 * Resetuje čtecí hlavu/y
 * @param bits
 */
void resetHeads(Bits *bits);


/**
 * Převede string na bit stream (zkontroluje, že se jedná o správný stream -> použití adaptive/static, použité modelu)
 * @param str
 * @param adaptive
 * @param model
 * @return
 */
Bits *loadBits(std::string *str, bool adaptive, bool model);


/**
 * Uží jeden bit do bit streamu
 * @param bits
 * @param bit
 */
void pushBit(Bits *bits, bool bit);

/**
 * Inicializuej bit stream
 * @param bits
 */
Bits *createBits(bool adaptive, bool model);

/**
 * Uloží číslodo bit streamu
 * @param bits
 * @param number
 */
void pushNumber(Bits *bits, unsigned int number);


/**
 * Přečte číslo z bit streamu
 * @param bits
 * @return
 */
unsigned char readNumber(Bits *bits);

/**
 * Uloží kod charu
 * @param bits
 * @param code
 */
void pushCode(Bits *bits, std::vector<bool> *code);

#endif //PROJEKT_BITS_H
