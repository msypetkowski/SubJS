#pragma once
#include "atom.h"

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;


class Lexer {
    vector<Atom*> atoms;
    vector<std::pair<unsigned,unsigned>> errors;
    string code;
    unsigned nextNewSymbolId;
    map<string, int> symbolsMap;

    vector<int> lastEndl;
    vector<int> line;

    // state variables
    unsigned pos;
    bool treatEndlAsSeparator;

    bool isWhitespace(char);
    bool isSpecial(char);
    bool getKeyword();
    bool getSymbol();
    bool getConstant();
    bool getComment();

public:
    Lexer(const string&);

    Atom* getNextAtom();

    vector<Atom*> getAtoms();
    vector<std::pair<unsigned,unsigned>> getErrors();

    // run lexical analysis to the end of file
    // results can be get using getAtoms
    // and getErrors methods
    bool run();
};
