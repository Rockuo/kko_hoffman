//
// Created by rockuo on 01.05.19.
//

#ifndef PROJEKT_MODEL_H
#define PROJEKT_MODEL_H


class Model {
public:
    virtual unsigned char getModifiedValue(unsigned char c) = 0;
    virtual unsigned char getOriginalValue(unsigned char c) = 0;
    virtual ~Model() = default;
};

class NoModel : virtual public Model {

public:
    unsigned char getModifiedValue(unsigned char c) override { return c; };
    unsigned char getOriginalValue(unsigned char c) override { return c; };
};

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
