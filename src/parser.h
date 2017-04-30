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

    // parsing tree node methods
    void DeclarationType();
    void Declaration();
    void DeclarationElem();
    void DeclarationNext();

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
