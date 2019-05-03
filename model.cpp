/**
 * Richard Bureš (xbures29)
 * Implementace modelů (žádný, nebo Diff)
 */
#include "model.h"

/**
 * Získá upravenou hodnotu ze znaku dle modelu
 * @param c
 * @return
 */
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

/**
 * Získá z upravené hodnoty znak
 * @param c
 * @return
 */
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

/**
 * Vytvoří model, buď Diff, nebo neupravený
 * @param enabled
 * @return
 */
Model *createModel(bool enabled) {
    return enabled
           ? static_cast<Model *> (new DiffModel())
           : static_cast<Model*> (new NoModel());
}