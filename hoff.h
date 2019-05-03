/**
 * Richard Bureš (xbures29)
 * Interface statickeho hofmanova kodovani
 */

#include <string>
#include <vector>
#include <map>

#ifndef PROJEKT_HOFF_H
#define PROJEKT_HOFF_H

/**
 * Kmprimuje/dekomprimuje soubor statiským hoff kodovanim
 * @param inputFileName
 * @param outputFileName
 * @param encode
 * @param modelEnabled
 */
void hoffman(const std::string &inputFileName, const std::string &outputFileName, bool encode, bool modelEnabled);


/**
 * Zístká strom z vah jednotlivých znaků
 * @param ranks
 * @return
 */
Node *getHoffmanTreeByRanks(std::map<unsigned char, int> *ranks);

/**
 * Převede strom na mapuc [char->jeho kód]
 * @param root
 * @param currentPath
 * @param result
 */
void getCodeMap(Node *root, std::vector<bool> *currentPath, std::map<unsigned char, std::vector<bool>> *result);
#endif //PROJEKT_HOFF_H
