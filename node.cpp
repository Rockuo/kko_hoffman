//
// Created by rockuo on 29.04.19.
//

#include "node.h"

Node *createEmptyNode() {
    Node *node = new Node;
    node->leaf = false;
    return node;
}
Node *createLeaf(const int weight, const unsigned char value) {
    Node *leaf = new Node;
    leaf->weight = weight;
    leaf->value = value;
    return leaf;
}