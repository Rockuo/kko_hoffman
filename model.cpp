//
// Created by rockuo on 01.05.19.
//
#include "model.h"

unsigned char DiffModel::getModifiedValue(unsigned char c) {
    unsigned char res;
    if (used) {
        res = lastChar - c; // takto je možné provést operaci oběma směry, pomocé stejného výpočtu
    } else {
        res = c;
        used = true;
    }
    lastChar = c;
    return res;
}

unsigned char DiffModel::getOriginalValue(unsigned char c) {
    unsigned char res;
    if (used) {
        res = lastChar - c; // takto je možné provést operaci oběma směry, pomocí stejného výpočtu
    } else {
        res = c;
        used = true;
    }
    lastChar = res;
    return res;
}

Model *createModel(bool enabled) {
    return enabled
           ? static_cast<Model *> (new DiffModel())
           : static_cast<Model*> (new NoModel());
}