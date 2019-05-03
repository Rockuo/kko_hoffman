/**
 * Richard Bureš (xbures29)
 * Chyby při kontrole vstupu
 */

#ifndef PROJEKT_ERRORS_H
#define PROJEKT_ERRORS_H

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
