/**
 * Richard Bureš (xbures29)
 * Hlavní soubor - zpracuje argumenty, spustí požadovanou akci
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
#include "errors.h"
#include "hoff.h"
#include "ahoff.h"
#include <cstdlib>

using namespace std;

/**
 * struktura argumentů
 */
struct arguments {
    unsigned int widthvalue = 1024;
    bool encode = true;
    bool adaptive = false;
    bool model = false;
    bool help = false;
    string inputFile;
    string outputFile;

};

/**
 * zpracuje argumenty do struktury argumentů
 * @param args
 * @return
 */
arguments getArguments(vector<string> args) {
    arguments res;
    res.help = false;

    string prev;

    for (const string &arg: args) {
        if (prev == "-w") {
            res.widthvalue = stoi(arg);
            prev = "";
        } else if (prev == "-h") {
            if (arg == "static") {
                res.adaptive = false;
            } else if (arg == "adaptive") {
                res.adaptive = true;
            } else {
                res.help = true;
                return res;
            }
            prev = "";
        } else if (prev == "-i") {
            res.inputFile = arg;
            prev = "";
        } else if (prev == "-o") {
            res.outputFile = arg;
            prev = "";
        } else if (arg == "-c") {
            res.encode = true;
        } else if (arg == "-d") {
            res.encode = false;
        } else if (arg == "-m") {
            res.model = true;
        } else {
            prev = arg;
        }
    }
    return res;
}


int main(int argc, char **argv) {

    arguments args = getArguments(std::vector<string>(argv, argv + argc));

    if (args.help) {
        cout << "Komprimace pomocí hoffmanova kodovani:" << endl;
        cout << "-h :help" << endl;
        cout << "-h static :staticke kodovani" << endl;
        cout << "-h static :adaptivni kodovani" << endl;
        cout << "-m :aktivuje differencialni model" << endl;
        cout << "-m :aktivuje differencialni model" << endl;
        return 0;
    }

    try {
        if (args.adaptive) {
            adaptiveHoffman(args.inputFile, args.outputFile, args.encode, args.model, (unsigned int)args.widthvalue);
        } else {
            hoffman(args.inputFile, args.outputFile, args.encode, args.model);
        }
    } catch (InvalidAdaptiveOption &e) {
        cerr << "Error: ";
        cerr << e.what() << endl;
    } catch (InvalidModelOption &e) {
        cerr << "Error: ";
        cerr << e.what() << endl;
    }
    return 0;
}