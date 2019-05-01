//
// Created by rockuo on 29.04.19.
//

#include <string>
#include <vector>
#include <map>

#ifndef PROJEKT_HOFF_H
#define PROJEKT_HOFF_H
void hoffman(const std::string &inputFileName, const std::string &outputFileName, bool encode, bool modelEnabled);
//Node *getTree(std::map<unsigned char, std::vector<bool>> *codeMap);
Node *getHoffmanTreeByRanks(std::map<unsigned char, int> *ranks);
void getCodeMap(Node *root, std::vector<bool> *currentPath, std::map<unsigned char, std::vector<bool>> *result);
#endif //PROJEKT_HOFF_H
