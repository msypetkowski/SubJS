#include "parser.h"

#include <iostream>
#include <cassert>

Parser::Parser(Lexer& l):lexer(l) {
    nextAtom();
}

void Parser::nextAtom() {
    while(!(curAtom=lexer.getNextAtom())) {
        errorsPositions.push_back(lexer.getLastError());
        errorsMessages.push_back("Lexer error");
        // TODO: skip atoms
    }
    assert(curAtom!=nullptr);
}

void Parser::acceptKeyword(const string& repr) {
    if (dynamic_cast<AtomKeyword*>(curAtom) &&
            curAtom->getRepr() == repr) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        // TODO: error handling
        std::cout<< "Expected keyword: " << repr << " got: " << curAtom->getRepr() << std::endl;
        exit(1);
    }
}

void Parser::acceptConstant() {
    if (dynamic_cast<AtomConstant*>(curAtom)) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        // TODO: error handling
        std::cout<<"Expected Constant"<<std::endl;
        exit(1);
    }
}
void Parser::acceptSymbol() {
    if (dynamic_cast<AtomSymbol*>(curAtom)) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        // TODO: error handling
        std::cout<<"Expected Symbol"<<std::endl;
        exit(1);
    }
}

bool Parser::isCurAtomKeyword(const string& repr) {
    return dynamic_cast<AtomKeyword*>(curAtom)
        && curAtom->getRepr()==repr;
}


/*
Program
    = { Block }
    | epsilon
*/
void Parser::Program() {
    tb.treeNodeStart("Program");
    while (!isCurAtomKeyword("$")) {
        Block();
    }
    tb.treeNodeEnd();
}

/*
Block
    = Declaration
    | OneLineCommaOperator OneLineCommaOperatorSeparator
    | Separator

    | Symbol ExpressionRest
    | Array ExpressionRest
    | CommaOperator ExpressionRest
    | new Expression
    | FunctionExpression

    | IfStatement
    | While
    | DoWhile
    | For
    | Try

    | continue
    | break
    | return Expression

    | ';'
 */
// TODO: implement whole
void Parser::Block() {
    tb.treeNodeStart("Block");
    if (isCurAtomKeyword("var")) {
        Declaration();
    } else  if (isCurAtomKeyword(";")) {
        acceptKeyword(";");
    }
    tb.treeNodeEnd();
}

/*
Declaration
    = DeclarationType DeclarationElem { ',' DeclarationElem }
DeclarationType
    = var
    | let
    | const
DeclarationElem
    = ident '=' Expression
    | ident
*/
void Parser::Declaration() {
    tb.treeNodeStart("Declaration");
    DeclarationType();
    DeclarationElem();
    while (curAtom->getRepr() == ",") {
        acceptKeyword(",");
        DeclarationElem();
    }
    tb.treeNodeEnd();
}
void Parser::DeclarationType() {
    tb.treeNodeStart("DeclarationType");
    if (curAtom->getRepr() == "var") {
        acceptKeyword("var");
    } else if (curAtom->getRepr() == "let") {
        acceptKeyword("let");
    } else if (curAtom->getRepr() == "const") {
        acceptKeyword("const");
    }
    tb.treeNodeEnd();
}
void Parser::DeclarationElem() {
    tb.treeNodeStart("DeclarationElem");
    acceptSymbol();
    if (curAtom->getRepr() == "=") {
        // TODO: Expression here
        acceptKeyword("=");
        acceptConstant();
    }
    tb.treeNodeEnd();
}
