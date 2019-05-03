/**
 * Richard Bureš (xbures29)
 * Adaptivní hofmanovo kodovani
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "node.h"
#include "bits.h"
#include "hoff.h"
#include "model.h"
#include "ahoff.h"


using namespace std;

/**
 * Uloží parametr -w do výslednéh bit streamu
 * @param bits (bit stream)
 * @param distance  (parametr -w)
 */
void saveDistance(Bits *bits, unsigned int distance) {
    pushBit(bits, ((distance >> 15u) & 1u) == 1u);
    pushBit(bits, ((distance >> 14u) & 1u) == 1u);
    pushBit(bits, ((distance >> 13u) & 1u) == 1u);
    pushBit(bits, ((distance >> 12u) & 1u) == 1u);
    pushBit(bits, ((distance >> 11u) & 1u) == 1u);
    pushBit(bits, ((distance >> 10u) & 1u) == 1u);
    pushBit(bits, ((distance >> 9u) & 1u) == 1u);
    pushBit(bits, ((distance >> 8u) & 1u) == 1u);
    pushBit(bits, ((distance >> 7u) & 1u) == 1u);
    pushBit(bits, ((distance >> 6u) & 1u) == 1u);
    pushBit(bits, ((distance >> 5u) & 1u) == 1u);
    pushBit(bits, ((distance >> 4u) & 1u) == 1u);
    pushBit(bits, ((distance >> 3u) & 1u) == 1u);
    pushBit(bits, ((distance >> 2u) & 1u) == 1u);
    pushBit(bits, ((distance >> 1u) & 1u) == 1u);
    pushBit(bits, ((distance >> 0u) & 1u) == 1u);
}

/**
 * Načte parametr -w ze vstupního (zkomprimovaného) bit streamu
 * @param bits
 * @return
 */
unsigned int loadDistance(Bits *bits) {
    unsigned int distance = 0;
    if (readBit(bits)) {
        distance = (1u << 15u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 14u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 13u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 12u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 11u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 10u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 9u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 8u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 7u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 6u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 5u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 4u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 3u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 2u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 1u) | distance;
    }
    if (readBit(bits)) {
        distance = (1u << 0u) | distance;
    }
    return distance;
}

/**
 * Zakóduje vstupní soubor do bit streamu pomocí adaptivního hofmanova kodovani
 * @param inputFileName
 * @param modelEnabled (byl zadán parametr -m)
 * @param distance
 * @return
 */
Bits *adaptiveHoffmanEncode(const string &inputFileName, bool modelEnabled, unsigned int distance) {
    ifstream inStream(inputFileName);

    // inicializuje model na rozdílový, nebo žádný
    Model *model = createModel(modelEnabled);

    if (inStream.fail()) {
        cerr << "Soubor " << inputFileName << " nelze otevřít.";
        exit(1); // terminate with error
    }

    // vytvoření inicializačních vah
    map<unsigned char, int> ranks;
    for (unsigned int i = 0; i < 256; i++) {
        ranks[(unsigned char) i] = 0;
    }

    // deklarace roměnných
    unsigned char c_n;
    unsigned char c;
    Node *root;
    auto *codeMap = new std::map<unsigned char, std::vector<bool>>;
    auto *tmpPath = new vector<bool>;

    // Inicializuje bit stream
    Bits *bits = createBits(true, modelEnabled);
    // uložé parametr -w
    saveDistance(bits, distance);

    // počítadlo kdy přegenerovat kodovani
    int i = 0;

    while (inStream >> noskipws >> c_n) {
        // načte char dle použitého modelu
        c = model->getModifiedValue(c_n);
        // pokud vypršelo počítadlo, přegeneruje strom a kodemap, dle již zpracovaných dat
        if (i == 0) {
            root = getHoffmanTreeByRanks(&ranks);
            getCodeMap(root, tmpPath, codeMap);
            delete root;
            i = distance;
        }
        // uloží zakodovaný char a to jaký char přibil do vah
        pushCode(bits, &(*codeMap)[c]);
        ranks[c]++;
        i--;
    }
    delete tmpPath;

    inStream.close();

    return bits;
}

/**
 * Dekoduje soubor zomprimovany adaptivnim hofmanovym kodovanim
 * @return
 */
string adaptiveHoffmanDecode(Bits *bits, bool modelEnabled) { //todo return pointer OR print directly
    unsigned int distance = loadDistance(bits);

    stringstream ss;
    //inicializuje model, na žádný nebo rozdílový
    Model *model = createModel(modelEnabled);

    // inicializuje defaltní ranking
    map<unsigned char, int> ranks;
    for (unsigned int i = 0; i < 256; i++) {
        ranks[(unsigned char) i] = 0;
    }

    // inicializace defaultniho stromu
    Node *root = getHoffmanTreeByRanks(&ranks);
    unsigned char c;
    Node *currentRoot = root;

    // pčítadlo kdy přegenerovat strom
    int i = distance;

    while (!bitsEnd(bits)) {
        if (currentRoot->leaf) { // jsem v listu -> rozkoduji char
            c = model->getOriginalValue(currentRoot->value);
            ss << c;
            ranks[currentRoot->value]++;
            i--;
            if (i == 0) { // po rokodování -w počtu charů přeeneruje strom
                delete root;
                root = getHoffmanTreeByRanks(&ranks);
                i = distance;
            }
            currentRoot = root;
        }
        // pruchod stromem
        bool goRight = readBit(bits);
        if (goRight) {
            currentRoot = currentRoot->right;
        } else {
            currentRoot = currentRoot->left;
        }

    }
    // na kondi jsme vlistu (melo by být vždy true)
    // tak rozkodovat poslední znak
    if (currentRoot->leaf) {
        c = model->getOriginalValue(currentRoot->value);
        ss << c;
    }
    delete root;

    return ss.str();
}

/**
 * Spustí komprimovani/dekomprimovani dle adaptivniho hofmanova kodovani
 * @param inputFileName
 * @param outputFileName
 * @param encode (true -> komprimace)
 * @param modelEnabled (true -> zapne mode)
 * @param distance (prametr -w)
 */
void adaptiveHoffman(const string &inputFileName, const string &outputFileName, bool encode, bool modelEnabled, unsigned int distance) {
    std::ofstream out(outputFileName);
    if (encode) {
        Bits *encoded = adaptiveHoffmanEncode(inputFileName, modelEnabled, distance);
        out << saveBits(encoded);
    } else {
        ifstream inFile;

        inFile.open(inputFileName);
        if (!inFile || inFile.fail()) {
            cerr << "Soubor " << inputFileName << " nelze otevřít.";
            exit(1);
        }

        std::string content((std::istreambuf_iterator<char>(inFile)),
                            std::istreambuf_iterator<char>());

        inFile.close();
        Bits *loaded = loadBits(&content, true, modelEnabled);
        out << adaptiveHoffmanDecode(loaded, modelEnabled);
    }
    out.close();
}
