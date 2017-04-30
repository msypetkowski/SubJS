#include "parser.h"

#include <iostream>
#include <cassert>

Parser::Parser(Lexer& l):lexer(l) {
    nextAtom();
}

// void Parser::treeNodeLogStart(const string&){
// }
// void Parser::treeNodeLogEnd(){
// }
// void Parser::treeNodeLogAtom(){
// }
// string Parser::getTreeStr(){
//     return "";
// }

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


/*
Program
    = Declaration Program
    | OneLineCommaOperator OneLineCommaOperatorSeparator Program
    | Separator Program

    | Symbol ExpressionRest DeclarationSeparator Program
    | Array ExpressionRest DeclarationSeparator Program
    | CommaOperator ExpressionRest DeclarationSeparator Program
    | new Expression DeclarationSeparator Program
    | FunctionExpression Program

    | IfStatement Program
    | While Program
    | DoWhile Program
    | For Program
    | Try Program

    | continue Separator
    | break Separator
    | return Expression Separator
Separator
    = ';'
    | epsilon
*/

// TODO: implement whole
void Parser::Program() {
    tb.treeNodeStart("Program");
    if (curAtom->getRepr() == "$") {
        std::cout<<"end"<<std::endl;
    } else if (curAtom->getRepr() == ";") {
        acceptKeyword(";");
        Program();
    } else if (curAtom->getRepr() == "var") {
        Declaration();
    }
    tb.treeNodeEnd();
}


/*
DeclarationType
    = var
    | let
    | const
Declaration
    = DeclarationType DeclarationElem { ',' DeclarationElem }
DeclarationElem
    = ident '=' Expression
    | ident
*/
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
