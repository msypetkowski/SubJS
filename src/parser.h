#pragma once

#include "lexer.h"
#include "tree.h"

class Parser {
    Lexer& lexer;
    Atom* curAtom;

    void nextAtom();
    void acceptKeyword(const string&);
    void acceptConstant();
    void acceptSymbol();

    bool isCurAtomKeyword(const string&);

    // parsing tree node methods
    void Block();
    void Declaration();
    void DeclarationType();
    void DeclarationElem();

    vector<std::pair<unsigned,unsigned>> errorsPositions;
    vector<string> errorsMessages;

    TreeBuilder tb;

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
