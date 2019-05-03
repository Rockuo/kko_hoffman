/**
 * Richard Bureš (xbures29)
 * Implementace statickeho hofmanova kodovani
 */


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include "bits.h"
#include "node.h"
#include "model.h"

using namespace std;

/**
 * Podmínka jestli jít při vkládání uzlu ve strome vlevo nebo vpravo
 * @param root
 * @return
 */
bool shouldPlaceLeft(Node *root) {
    return root->left->weight < root->right->weight || ( // levá je jasně menší než pravá, nebo
            root->left->weight == root->right->weight && // levá je stejně velka, ale k tomu
            root->left->childLength <= root->right->childLength // levá má méně potomků nebo stejně potomků jako pravá
            //(takže bude strom vyváženější když se pujde do leva, nebo je to jedno a tedy levá je default)
    );
}

/**
 * Podmínka jestli při vkládání uzlu jít dolu stromem, nebo nový uzel vleput nad aktuální root
 * @param root
 * @param leaf
 * @return
 */
bool shouldExtendTree(Node *root, Node *leaf) {
    return root->leaf || // root je list (nemělo by to být třeba ošetřovat, ale tak kdyby náhodou), nebo
           root->weight < leaf->weight || // nový list je větší rovno aktuálnímu podstromu, nebo
           ( // nebo jsou oba podstromy menší rovno
                   root->left->weight < leaf->weight &&
                   root->right->weight < leaf->weight
           );

}

/**
 * Přidá list do stromu
 * @param root
 * @param leaf
 * @return
 */
Node *treeAdd(Node *root, Node *leaf) {
    Node *newRoot = root;
    // strom má 0 prvků
    if (root->left == nullptr && !root->leaf) {
        root->left = leaf;
        root->weight = leaf->weight;

    }
    // strom má 1 prvek
    else if (root->right == nullptr && !root->leaf) {
        root->right = leaf;
        root->weight = root->weight + leaf->weight;
    }
    // umistění nad
    else if (shouldExtendTree(root, leaf)) {
        newRoot = new Node;
        newRoot->leaf = false;
        newRoot->left = root;
        newRoot->right = leaf;
        newRoot->childLength = root->childLength + 1;
        newRoot->weight = root->weight + leaf->weight;
    } else { // else budeme strom rozdělovat (pro jasnou implikaci, že to je velký rozdíl nedáváme elsif)
        if (shouldPlaceLeft(root)) {
            root->left = treeAdd(root->left, leaf);
        } else {
            root->right = treeAdd(root->right, leaf);
        }
        root->weight = root->left->weight + root->right->weight;
        root->childLength = root->left->childLength + root->right->childLength;
    }
    return newRoot;
}

/**
 * Převede strom na mapuc [char->jeho kód]
 * @param root
 * @param currentPath
 * @param result
 */
void getCodeMap(Node *root, vector<bool> *currentPath, map<unsigned char, vector<bool>> *result) {
    if (root->leaf) {
        (*result)[root->value] = (*currentPath);
        return;
    }

    if (root->left != nullptr) {
        currentPath->push_back(false);
        getCodeMap(root->left, currentPath, result);
        currentPath->pop_back();
    }

    if (root->right != nullptr) {
        currentPath->push_back(true);
        getCodeMap(root->right, currentPath, result);
        currentPath->pop_back();
    }
}

/**
 * Uloží list obsahem "c" na cestu stromem určenou "code"
 * @param root
 * @param code
 * @param c
 * @param index
 */
void fillTree(Node *root, vector<bool> *code, unsigned char c, unsigned int index) {
    if (code->size() == (index + 1u)) // příští iteraci by byl konec
    {
        if ((*code)[index]) { // go right
            root->right = createLeaf(0, c);
        } else {
            root->left = createLeaf(0, c);
        }
    } else {
        if (root->leaf) {
            throw invalid_argument("Nevalidní enkodovaný soubor");
        }
        if ((*code)[index]) { // go right
            if (root->right == nullptr) {
                root->right = createEmptyNode();
            }
            fillTree(root->right, code, c, index + 1u);
        } else {
            if (root->left == nullptr) {
                root->left = createEmptyNode();
            }
            fillTree(root->left, code, c, index + 1u);
        }
    }

}

/**
 * Převede maování (char->kód) na strom
 * @param codeMap
 * @return
 */
Node *getTree(map<unsigned char, vector<bool>> *codeMap) {
    Node *root = createEmptyNode();
    for (pair<unsigned char const, vector<bool>> m: (*codeMap)) {
        if (!m.second.empty()) {
            fillTree(root, &m.second, m.first, 0u);
        }
    }
    return root;
}

/**
 * Zístká strom z vah jednotlivých znaků
 * @param ranks
 * @return
 */
Node *getHoffmanTreeByRanks(map<unsigned char, int> *ranks) {

    map<int, vector<unsigned char>> reverseRanks;
    vector<int> sortedRanks;
    for (pair<unsigned char const, int> r : (*ranks)) {
        if (reverseRanks.find(r.second) == reverseRanks.end()) {
            vector<unsigned char> v;
            reverseRanks[r.second] = v;
            sortedRanks.push_back(r.second);
        }
        reverseRanks[r.second].push_back(r.first);
    }
    sort(sortedRanks.begin(), sortedRanks.end());

    Node *root = new Node;
    root->leaf = false;
    for (const int rank: sortedRanks) {
        for (unsigned char value: reverseRanks[rank]) {
            root = treeAdd(root, createLeaf(rank, value));
        }
    }
    return root;
}

/**
 * vygeneruje strom z obsahu kodovaneho souboru
 * @param content
 * @param modelEnabled
 * @return
 */
Node *getHoffmanTree(string *content, bool modelEnabled) {
    Model *model = createModel(modelEnabled);

    map<unsigned char, int> ranks;
    vector<int> used;
    for (unsigned char c_n : (*content)) {
        unsigned char c = model->getModifiedValue(c_n);
        if (ranks.find(c) == ranks.end()) {
            used.push_back((int) c);
            ranks[c] = 0;
        }
        ranks[c]++;
    }

    delete model;

    return getHoffmanTreeByRanks(&ranks);
}

/**
 * Uložé celé mapování (char->code) do bitstreamu
 * @param bits
 * @param codeMap
 */
void pushCodeMap(Bits *bits, map<unsigned char, vector<bool>> *codeMap) {
    vector<bool> currentCode;
    for (unsigned int i = 0; i < 256; i++) {
        auto c = (unsigned char) i;

        if (codeMap->find(c) == codeMap->end()) {
            pushNumber(bits, 0u);
        } else {
            currentCode = (*codeMap)[c];
            pushNumber(bits, (unsigned int) currentCode.size());
            pushCode(bits, &(currentCode));
        }
    }
}

/**
 * Načte mapování (char->code) z bit streamu
 * @param bits
 * @return
 */
map<unsigned char, vector<bool>> *popCodeMap(Bits *bits) {
    auto *codeMap = new map<unsigned char, vector<bool>>;
    for (unsigned int i = 0; i < 256; i++) {
        auto c = (unsigned char) i;
        unsigned char size = readNumber(bits);
        if (size != 0) {
            vector<bool> currentCode(size);
            for (int j = 0; j < size; j++) {
                currentCode[j] = readBit(bits);
            }
            (*codeMap)[c] = currentCode;
        }
    }
    return codeMap;
}

/**
 * Zakoduje řetězec statiským hofmanovym kodováním a vráti jeho bitstream
 * @param contentPtr
 * @param modelEnabled
 * @return
 */
Bits *hoffmanEncode(string *contentPtr, bool modelEnabled) {
    Node *root = getHoffmanTree(contentPtr, modelEnabled);

    Model *model = createModel(modelEnabled);

    auto codeMap = new map<unsigned char, vector<bool>>;
    auto tmpPath = new vector<bool>;
    getCodeMap(root, tmpPath, codeMap);
    delete root;
    delete tmpPath;

    Bits *bits = createBits(false, modelEnabled);
    pushCodeMap(bits, codeMap);

    for (const unsigned char c_n: (*contentPtr)) {
        unsigned char c = model->getModifiedValue(c_n);
        pushCode(bits, &((*codeMap)[c]));
    }

    delete model;
    delete codeMap;

    return bits;
}


/**
 * Dekoduje bit stream ískaný statickým hofmanovým kodováním
 * @param bits
 * @param modelEnabled
 * @return
 */
string hoffmanDecode(Bits *bits, bool modelEnabled) {
    stringstream ss;

    Model *model = createModel(modelEnabled);

    map<unsigned char, vector<bool>> *newMap = popCodeMap(bits);
    Node *root = getTree(newMap);

    Node *currentRoot = root;

    while (!bitsEnd(bits)) {
        if (currentRoot->leaf) {
            char c = model->getOriginalValue(currentRoot->value);
            ss << c;
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
        ss << model->getOriginalValue(currentRoot->value);
    }


    delete root;
    delete newMap;
    delete model;

    return ss.str();
}

/**
 * Kmprimuje/dekomprimuje soubor statiským hoff kodovanim
 * @param inputFileName
 * @param outputFileName
 * @param encode
 * @param modelEnabled
 */
void hoffman(const string &inputFileName, const string &outputFileName, bool encode, bool modelEnabled) {
    ifstream inFile;


    inFile.open(inputFileName);
    if (!inFile) {
        cerr << "Soubor " << inputFileName << " nelze otevřít.";
        exit(1); // terminate with error
    }

    std::string content((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());

    inFile.close();

    std::ofstream out(outputFileName);

    // code/decode
    if (encode) {
        Bits *encoded = hoffmanEncode(&content, modelEnabled);
        out << saveBits(encoded);
        delete encoded;
    } else {
        Bits *loaded = loadBits(&content, false, modelEnabled);
        out << hoffmanDecode(loaded, modelEnabled);
        delete loaded;
    }
    out.close();
}
