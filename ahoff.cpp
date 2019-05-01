//
// Created by rockuo on 01.05.19.
//

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

Bits *adaptiveHoffmanEncode(const string &inputFileName, bool modelEnabled, int distance) {
    ifstream inStream(inputFileName);

    Model *model = createModel(modelEnabled);

    if (inStream.fail()) {
        cerr << "Soubor " << inputFileName << " nelze otevřít.";
        exit(1); // terminate with error
    }

    map<unsigned char, int> ranks;
    for (unsigned int i = 0; i < 256; i++) {
        ranks[(unsigned char) i] = 0;
    }


    unsigned char c_n;
    unsigned char c;
    Node *root;
    auto *codeMap = new std::map<unsigned char, std::vector<bool>>;
    auto *tmpPath = new vector<bool>;

    Bits *bits = createBits(true, modelEnabled);

    int i = 0;

    while (inStream >> noskipws >> c_n) {
        c = model->getModifiedValue(c_n);
        if (i == 0) {
            root = getHoffmanTreeByRanks(&ranks);
            getCodeMap(root, tmpPath, codeMap);
            delete root;
            i = distance;
        }

        pushCode(bits, &(*codeMap)[c]);
        ranks[c]++;
        i--;
    }
    delete tmpPath;

    inStream.close();

    return bits;
}

/**
 *
 * @return
 */
string adaptiveHoffmanDecode(Bits *bits, bool modelEnabled, int distance) { //todo return pointer OR print directly
    stringstream ss;

    Model *model = createModel(modelEnabled);

    auto *codeMap = new std::map<unsigned char, std::vector<bool>>;
    map<unsigned char, int> ranks;
    for (unsigned int i = 0; i < 256; i++) {
        ranks[(unsigned char) i] = 0;
    }

    auto *tmpPath = new vector<bool>;
    Node *root = getHoffmanTreeByRanks(&ranks);
    getCodeMap(root, tmpPath, codeMap);
    unsigned char c;
    Node *currentRoot = root;

    int i = distance;

    while (!bitsEnd(bits)) {
        if (currentRoot->leaf) {
            c = model->getOriginalValue(currentRoot->value);
            ss << c;
            ranks[currentRoot->value]++;
            i--;
            if (i == 0) {
                delete root;
                root = getHoffmanTreeByRanks(&ranks);
                getCodeMap(root, tmpPath, codeMap);
                i = distance;
            }
            currentRoot = root;
        }
        bool goRight = readBit(bits);
        if (goRight) {
            currentRoot = currentRoot->right;
        } else {
            currentRoot = currentRoot->left;
        }

    }
    if (currentRoot->leaf) {
        c = model->getOriginalValue(currentRoot->value);
        ss << c;
    }
    delete root;
    delete codeMap;

    return ss.str();
}


void adaptiveHoffman(const string &inputFileName, const string &outputFileName, bool encode, bool modelEnabled, int distance) {


    std::ofstream out(outputFileName);
    if (encode) {
        Bits *encoded = adaptiveHoffmanEncode(inputFileName, modelEnabled, distance);
        out << saveBits(encoded);
    } else {
        ifstream inFile;

        inFile.open(inputFileName);
        if (!inFile || inFile.fail()) {
            cerr << "Soubor " << inputFileName << " nelze otevřít.";
            exit(1); // terminate with error
        }

        std::string content((std::istreambuf_iterator<char>(inFile)),
                            std::istreambuf_iterator<char>());

        inFile.close();
        Bits *loaded = loadBits(&content, true, modelEnabled);
        out << adaptiveHoffmanDecode(loaded, modelEnabled, distance);
    }
    out.close();
}
