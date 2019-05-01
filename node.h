//
// Created by rockuo on 29.04.19.
//

#ifndef PROJEKT_NODE_H
#define PROJEKT_NODE_H
struct Node {

    ~Node()
    {
        delete left; delete right;
    }

    int weight = 0;
    bool leaf = true;
    Node *left = nullptr;
    Node *right = nullptr;
    int childLength = 0;
    unsigned char value;

};
Node *createEmptyNode();
Node *createLeaf(int weight, unsigned char value);
#endif //PROJEKT_NODE_H
