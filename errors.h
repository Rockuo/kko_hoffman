//
// Created by rockuo on 01.05.19.
//

#ifndef PROJEKT_ERRORS_H
#define PROJEKT_ERRORS_H

#define ADAPTIVE_ERROR  1u
#define MODEL_ERROR  2u

struct InvalidAdaptiveOption : public std::exception {
    const char * what () const noexcept override {
        return "Zvažte změnu přepínače -h";
    }
};


struct InvalidModelOption : public std::exception {
    const char * what () const noexcept override {
        return "Zvažte změnu přepínače -m";
    }
};
#endif //PROJEKT_ERRORS_H
