/**
 * Richard Bureš (xbures29)
 * Adaptivní hofmanovo kodovani (interface)
 */


#include <string>

#ifndef PROJEKT_AHOFF_H
#define PROJEKT_AHOFF_H
/**
 * Spustí komprimovani/dekomprimovani dle adaptivniho hofmanova kodovani
 * @param inputFileName
 * @param outputFileName
 * @param encode (true -> komprimace)
 * @param modelEnabled (true -> zapne mode)
 * @param distance (prametr -w)
 */
void adaptiveHoffman(const std::string &inputFileName, const std::string &outputFileName, bool encode, bool modelEnabled, unsigned int distance);
#endif //PROJEKT_AHOFF_H
