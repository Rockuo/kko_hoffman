/**
 * Richard Bureš (xbures29)
 * Jednoduche prace s uzly stromu
 */

#include "node.h"

/**
 * Vytvoří prázdný uzel
 * @return
 */
Node *createEmptyNode() {
    Node *node = new Node;
    node->leaf = false;
    return node;
}

/**
 * Vytvoří list
 * @param weight
 * @param value
 * @return
 */
Node *createLeaf(const int weight, const unsigned char value) {
    Node *leaf = new Node;
    leaf->weight = weight;
    leaf->value = value;
    return leaf;
}