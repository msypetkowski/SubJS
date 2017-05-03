#pragma once

#include "lexer.h"
#include "tree.h"

#include <iostream>

class Parser {
    friend class Synchronize;

    Lexer& lexer;
    Atom* curAtom;

    void nextAtom();
    void acceptKeyword(const string&);
    void acceptConstant();
    void acceptSymbol();

    bool isCurAtomKeyword(const string&);

    // parsing tree node methods
    void Block          (const SymSet& follow);
    void Declaration    (const SymSet& follow);
    void DeclarationType(const SymSet& follow);
    void DeclarationElem(const SymSet& follow);

    vector<std::pair<unsigned,unsigned>> errorsPositions;
    vector<string> errorsMessages;

    TreeBuilder tb;

    bool canParse;

    void notExpectedAtomError(const Atom* a) {
        errorsPositions.push_back(a->getPos());
        errorsMessages.push_back(
                "Not expected atom: " + a->getStr());
    }

public:
    Parser(Lexer&);

    // starting tree node method
    void Program();

    const vector<std::pair<unsigned,unsigned>>& getErrorsPositions() {
        return errorsPositions;
    }
    const vector<string>& getErrorsMessages() {
        return errorsMessages;
    }

    string getTreeStr() { return tb.getTreeStr(); }

};

class Synchronize
{
    const SymSet &follow;
    void skipto(const SymSet &sset) {
        while(!sset.has(parser->curAtom))
            parser->nextAtom();
    }

public:
    Parser *parser;
    Synchronize(Parser *p, const SymSet& ss, const SymSet& fs)
            :follow(fs), parser(p) {
        if(!ss.has(parser->curAtom)) {
            parser->notExpectedAtomError(parser->curAtom);
            skipto(ss+follow);
        }
        parser->canParse = ss.has(parser->curAtom);
    }

    ~Synchronize() {
        if(!follow.has(parser->curAtom)) {
            parser->notExpectedAtomError(parser->curAtom);
            skipto(follow);
        }
    }
};
