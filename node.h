/**
 * Richard Bureš (xbures29)
 * Uzel stromu
 */

#ifndef PROJEKT_NODE_H
#define PROJEKT_NODE_H

/**
 * Struktura uzlu stromu
 */
struct Node {

    ~Node()
    {
        // při mazaní smaže své potomky
        delete left; delete right;
    }

    int weight = 0;
    bool leaf = true;
    Node *left = nullptr;
    Node *right = nullptr;
    int childLength = 0;
    unsigned char value;

};

/**
 * Vytvoří prázdný uzel
 * @return
 */
Node *createEmptyNode();

/**
 * Vytvoří list
 * @param weight
 * @param value
 * @return
 */
Node *createLeaf(int weight, unsigned char value);
#endif //PROJEKT_NODE_H
