#define DEBUG_MODE false


#include "../lib/types.hpp"
#include <iostream>
#include <fstream>

using namespace std;

bool blockComment = false,
    lineComment = false;
unsigned char InterpretChar (char in) {
    if (lineComment) {
        if (in == '\n') { lineComment = false; return 16; }
        return 16;
    } 
    if (blockComment) {
        if (in == ';') { blockComment = false; return 16; }
        return 16;
    }
        
    switch (in) {
        case '0':
            return 0b0000;
        break;
        case '1':
            return 0b0001;
        break;
        case '2':
            return 0b0010;
        break;
        case '3':
            return 0b0011;
        break;
        case '4':
            return 0b0100;
        break;
        case '5':
            return 0b0101;
        break;
        case '6':
            return 0b0110;
        break;
        case '7':
            return 0b0111;
        break;
        case '8':
            return 0b1000;
        break;
        case '9':
            return 0b1001;
        break;
        case 'a':
        case 'A':
            return 0b1010;
        break;
        case 'b':
        case 'B':
            return 0b1011;
        break;
        case 'c':
        case 'C':
            return 0b1100;
        break;
        case 'd':
        case 'D':
            return 0b1101;
        break;
        case 'e':
        case 'E':
            return 0b1110;
        break;
        case 'f':
        case 'F':
            return 0b1111;
        break;
        case ';':
            blockComment = true;
            return 16;
        break;
        case ':':
            lineComment = true;
            return 16;
        break;
        default:
            return 16;
        break;
    }
}

i32 main(i32 argc, char* argv[]) {
    if (argc < 2) { cout << "No arguments found.\nUsage: binassembler <inputfile.doesntmatter> <outputfile.doesntmatter>" << endl; return 1; }
    if (argc > 3) { cout << "Too many arguments." << endl; return 1; }
    if (argc != 3) { cout << "Too few arguments.\nUsage: binassembler <inputfile.doesntmatter> <outputfile.doesntmatter>" << endl; return 1; }

    ofstream output(argv[2]);
    if (!output.is_open()) { cout << "Couldn't open output file." << endl; return 1; }
    ifstream input(argv[1]);
    if (!input.is_open()) { cout << "Couldn't open input file." << endl; return 1; }
    
    char inputChar = 0, assembledChar = 0;
    bool lowerNibble = false;

    while (!input.eof()) {
        input.get(inputChar);
#if DEBUG_MODE
        cout << "Read: " << inputChar << endl;
#endif
        if ((inputChar = InterpretChar(inputChar)) > 0b1111) continue;
#if DEBUG_MODE
        cout << "Interpreted: " << inputChar << endl;
#endif
        if (lowerNibble) {
            assembledChar |= inputChar;
            lowerNibble = false;
        } else {
            assembledChar |= inputChar << 4;
            lowerNibble = true;
        }
        if (!lowerNibble) {
#if DEBUG_MODE
            cout << "Assembled Char: " << assembledChar << endl;
#endif
            output.put(assembledChar);
            assembledChar = 0;
        }
    }
    assembledChar = 0;
    if (input.eof() && lowerNibble) { assembledChar |= 0b0000; output.put(assembledChar); }
    input.close();
    output.close();
    cout << "Output written into \"" << argv[2] << "\"." << endl;
    return 0;
}