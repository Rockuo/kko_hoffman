//
// Created by rockuo on 29.04.19.
//

#include <vector>
#include <string>

#ifndef PROJEKT_BITS_H
#define PROJEKT_BITS_H

struct Bits {

    std::vector<unsigned char> parts;
    unsigned char partHead = 255u; // 0->7 (255 značí přetečení do další části)
    unsigned char tail = 0u; // 0->8
    unsigned long int mainHead = 0u; // 0->partHead.size()-1
};


bool bitsEnd(Bits *bits);


bool readBit(Bits *bits);

std::string saveBits(Bits *bits);


void resetHeads(Bits *bits);

Bits *loadBits(std::string *str, bool adaptive, bool model);

void pushBit(Bits *bits, bool bit);

/**
 * Alokuje místo pro uložení tailu
 * @param bits
 */
Bits *createBits(bool adaptive, bool model);

void pushNumber(Bits *bits, unsigned int number);

unsigned char readNumber(Bits *bits);


void pushCode(Bits *bits, std::vector<bool> *code);

#endif //PROJEKT_BITS_H
