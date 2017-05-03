#pragma once
#include "source.h"
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
    Source code;
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

    void newKeywordAtom(int key) {
        AtomPos p {line[pos], pos - lastEndl[pos] - 1};
        atoms.push_back(new AtomKeyword(p, key));
    }
    void newSymbolAtom(const string& word) {
        AtomPos p {line[pos], pos - lastEndl[pos] - 1};
        atoms.push_back(new AtomSymbol(p, word, symbolsMap[word]));
    }
    void newConstantStringAtom(const string& word) {
        AtomPos p {line[pos], pos - lastEndl[pos] - 1};
        atoms.push_back(new AtomConstant(p, word, AtomConstant::Type::String));
    }
    void newConstantIntegerAtom(const string& word) {
        AtomPos p {line[pos], pos - lastEndl[pos] - 1};
        atoms.push_back(new AtomConstant(p, word, AtomConstant::Type::Integer));
    }
    void newConstantFloatAtom(const string& word) {
        AtomPos p {line[pos], pos - lastEndl[pos] - 1};
        atoms.push_back(new AtomConstant(p, word, AtomConstant::Type::Float));
    }

public:
    Lexer(Source&);

    Atom* getNextAtom();
    std::pair<unsigned,unsigned> getLastError();

    vector<Atom*> getAtoms();
    vector<std::pair<unsigned,unsigned>> getErrors();

    // run lexical analysis to the end of file
    // results can be get using getAtoms
    // and getErrors methods
    bool run();
};
