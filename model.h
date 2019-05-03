/**
 * Richard Bureš (xbures29)
 * Interface modelů (žádný, nebo Diff)
 */

#ifndef PROJEKT_MODEL_H
#define PROJEKT_MODEL_H


/**
 * Liboolný model který se dá použít
 */
class Model {
public:
    /**
     * Získá upravenou hodnotu ze znaku dle modelu
     * @param c
     * @return
     */
    virtual unsigned char getModifiedValue(unsigned char c) = 0;

    /**
     * Získá z upravené hodnoty znak
     * @param c
     * @return
     */
    virtual unsigned char getOriginalValue(unsigned char c) = 0;

    virtual ~Model() = default;
};

/**
 * Implementace modelu která nijak neupravuje vstupní hodnotu
 */
class NoModel : virtual public Model {

public:
    unsigned char getModifiedValue(unsigned char c) override { return c; };

    unsigned char getOriginalValue(unsigned char c) override { return c; };
};

/**
 * Implementace modelu, která získává rozdíl s předchozí hodnotou
 */
class DiffModel : virtual public Model {
private:
    unsigned char lastChar = 0;
    bool used = false;
public:
    unsigned char getModifiedValue(unsigned char c) override;

    unsigned char getOriginalValue(unsigned char c) override;
};

Model *createModel(bool enabled);

#endif //PROJEKT_MODEL_H
