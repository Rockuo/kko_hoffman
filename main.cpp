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

struct arguments {
    unsigned int widthvalue = 100;
    bool encode = true;
    bool adaptive = false;
    bool model = false;
    bool help = false;
    string inputFile;
    string outputFile;

};

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
        cout << "TODO HELP" << endl;
        return 0;
    }

    try {
        if (args.adaptive) {
            adaptiveHoffman(args.inputFile, args.outputFile, args.encode, args.model, args.widthvalue);
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