#include "parser.h"

#include <iostream>
#include <cassert>

Parser::Parser(Lexer& l):lexer(l), canParse(true) {
    nextAtom();
}

void Parser::nextAtom() {
    while(!(curAtom=lexer.getNextAtom())) {
        errorsPositions.push_back(lexer.getLastError());
        errorsMessages.push_back("Lexer error");
    }
    assert(curAtom!=nullptr);
}

void Parser::acceptKeyword(const string& repr) {
    if (dynamic_cast<AtomKeyword*>(curAtom) &&
            curAtom->getRepr() == repr) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        string msg = "Expected keyword: " + repr + " got: " + curAtom->getStr();
        errorsPositions.push_back(curAtom->getPos());
        errorsMessages.push_back(msg);
    }
}

void Parser::acceptConstant() {
    if (dynamic_cast<AtomConstant*>(curAtom)) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        string msg = "Expected Constant, got: " + curAtom->getStr();
        errorsPositions.push_back(curAtom->getPos());
        errorsMessages.push_back(msg);
    }
}
void Parser::acceptSymbol() {
    if (dynamic_cast<AtomSymbol*>(curAtom)) {
        tb.treeNodeAtom(curAtom);
        nextAtom();
    } else {
        string msg = "Expected Symbol, got: " + curAtom->getStr();
        errorsPositions.push_back(curAtom->getPos());
        errorsMessages.push_back(msg);
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
    SymSet first = {"var", "let", "const", ";", "$"};

    while (!isCurAtomKeyword("$")) {
        Block(first);
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
void Parser::Block(const SymSet& follow) {
    SymSet first = {"var", "let", "const", ";"};
    Synchronize(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Block");

    if (isCurAtomKeyword("var")) {
        Declaration(follow);
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
void Parser::Declaration(const SymSet& follow) {
    SymSet first{"var", "let", "const"};
    Synchronize(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Declaration");

    DeclarationType({"SYMBOL"});
    DeclarationElem(follow + SymSet{","});
    while (isCurAtomKeyword(",")) {
        acceptKeyword(",");
        DeclarationElem(follow + SymSet{","});
    }

    tb.treeNodeEnd();
}
void Parser::DeclarationType(const SymSet&) {
    tb.treeNodeStart("DeclarationType");
    if (isCurAtomKeyword("var")) {
        acceptKeyword("var");
    } else if (isCurAtomKeyword("let")) {
        acceptKeyword("let");
    } else if (isCurAtomKeyword("const")) {
        acceptKeyword("const");
    }
    tb.treeNodeEnd();
}
void Parser::DeclarationElem(const SymSet&) {
    tb.treeNodeStart("DeclarationElem");
    acceptSymbol();
    if (isCurAtomKeyword("=")) {
        // TODO: Expression here
        acceptKeyword("=");
        acceptConstant();
    }
    tb.treeNodeEnd();
}
