//
// Created by rockuo on 29.04.19.
//

#include <string>
#include <vector>
#include "bits.h"
#include "errors.h"


bool bitsEnd(Bits *bits) {
    return (bits->mainHead == (bits->parts.size() - 1) && bits->partHead == ((bits->tail - 1)%8)) ||
            (bits->mainHead == (bits->parts.size())) // failsafe
            ;
}


bool readBit(Bits *bits) {
    unsigned char part = (bits->parts)[bits->mainHead];
    bool result = ((part >> bits->partHead) & 1u) == 1u;

    if (bits->partHead == 0u) {
        bits->partHead = 7u;
        bits->mainHead++;
    } else {
        bits->partHead--;
    }
    return result;
}


std::string saveBits(Bits *bits) { // todo return ptr
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
    std::string str = std::string(bits->parts.begin(), bits->parts.end());

    return std::string(bits->parts.begin(), bits->parts.end());
}


void resetHeads(Bits *bits) {
    bits->partHead = 7u;
    bits->mainHead = 0u;
}

Bits *loadBits(std::string *str, bool adaptive, bool model) {
    Bits *bits = new Bits;
    resetHeads(bits);
    std::copy((*str).begin(), (*str).end(), std::back_inserter(bits->parts));

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


    if(readBit(bits) != adaptive)
    {
        throw  InvalidAdaptiveOption();
    }
    if(readBit(bits) != model)
    {
        throw InvalidModelOption();
    }

    return bits;
}

void pushBit(Bits *bits, bool bit) {
    if (bits->partHead == 255u) {
        bits->parts.push_back(0u);
        bits->partHead = 7u;
        bits->tail = 8u;
        bits->mainHead++;
    }
    if (bit) {
        bits->parts[bits->parts.size() - 1] = (1u << bits->partHead) | (bits->parts.back());
    }
    bits->partHead--;
    bits->tail--;
}

/**
 * Alokuje místo pro uložení tailu
 * @param bits
 */
Bits *createBits(bool adaptive, bool model) {
    Bits *bits = new Bits;
    pushBit(bits, false);
    pushBit(bits, false);
    pushBit(bits, false);
    pushBit(bits, false);

    pushBit(bits, adaptive);
    pushBit(bits, model);
    bits->mainHead--;
    return bits;
}


void pushNumber(Bits *bits, unsigned int number) {
    for (unsigned char i = 8u; i > 0u; i--) {
        pushBit(bits, ((number >> (i - 1u)) & 1u) == 1u);
    }
}

unsigned char readNumber(Bits *bits) {
    unsigned char result = 0u;
    for (unsigned char i = 8u; i > 0u; i--) {
        if (readBit(bits)) {
            result = (1u << (i - 1u)) | result;
        }
    }
    return result;
}


void pushCode(Bits *bits, std::vector<bool> *code) {
    for (bool b: (*code)) {
        pushBit(bits, b);
    }
}
