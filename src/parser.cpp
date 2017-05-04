#include "parser.h"

#include <iostream>
#include <cassert>

#define ASSIGNMENT_OPERATORS_STRINGS { \
    "=", "+=", "-=", "*=", "/=", "^=", "&=", "|=", "%=", \
}
#define ADDITIVE_OPERATORS_STRINGS { "+", "-" }
#define MULTIPLICATIVE_OPERATORS_STRINGS { "*", "/" }

const static SymSet ASSIGNMENT_OPERATORS(ASSIGNMENT_OPERATORS_STRINGS);
const static SymSet ADDITIVE_OPERATORS(ADDITIVE_OPERATORS_STRINGS);
const static SymSet MULTIPLICATIVE_OPERATORS(MULTIPLICATIVE_OPERATORS_STRINGS);

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
    = { Element }
    | epsilon
*/
void Parser::Program() {
    tb.treeNodeStart("Program");
    SymSet first = {"var", "let", "const", ";", "$"};

    while (!isCurAtomKeyword("$")) {
        Element(first);
    }

    tb.treeNodeEnd();
}

/*
Element
    = Declaration
    | OneLineCommaOperator OneLineCommaOperatorSeparator

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
void Parser::Element(const SymSet& follow) {
    SymSet first = {"var", "let", "const", ";"};
    Synchronize(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Element");

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
    = ident '=' AssignmentExpression
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
void Parser::DeclarationElem(const SymSet& follow) {
    tb.treeNodeStart("DeclarationElem");
    acceptSymbol();
    if (isCurAtomKeyword("=")) {
        acceptKeyword("=");
        AssignmentExpression(follow);
    }
    tb.treeNodeEnd();
}


/*
Expression
    = AssignmentExpression
    | AssignmentExpression "," Expression

AssignmentExpression
    = ConditionalExpression
    | ConditionalExpression AssignmentOperator AssignmentExpression

ConditionalExpression
    = OrExpression
    | OrExpression "?" AssignmentExpression ":" AssignmentExpression

OrExpression
    = AndExpression
    | AndExpression "||" OrExpression

AndExpression
    = BitwiseOrExpression
    | BitwiseOrExpression "&&" AndExpression

BitwiseOrExpression
    = BitwiseXorExpression
    | BitwiseXorExpression "|" BitwiseOrExpression

BitwiseXorExpression
    = BitwiseAndExpression
    | BitwiseAndExpression "^" BitwiseXorExpression

BitwiseAndExpression
    = EqualityExpression
    | EqualityExpression "&" BitwiseAndExpression

EqualityExpression
    = RelationalExpression
    | RelationalExpression EqualityOperator EqualityExpression

RelationalExpression
    = ShiftExpression
    | RelationalExpression RelationalOperator ShiftExpression

ShiftExpression
    = AdditiveExpression
    | AdditiveExpression ShiftOperator ShiftExpression

AdditiveExpression
    = MultiplicativeExpression
    | MultiplicativeExpression AdditiveOperator AdditiveExpression

MultiplicativeExpression
    = UnaryExpression
    | UnaryExpression MultiplicativeOperator MultiplicativeExpression

*/

void Parser::Expression                 (const SymSet& follow) {
    tb.treeNodeStart("Expression");
    AssignmentExpression(follow + SymSet{","});
    if (isCurAtomKeyword(",")) {
        Expression(follow);
    }
    tb.treeNodeEnd();
}
void Parser::AssignmentExpression       (const SymSet& follow) {
    tb.treeNodeStart("AssignmentExpression");
    // TODO: ConditionalExpression here
    AdditiveExpression(follow + ASSIGNMENT_OPERATORS);
    if (ASSIGNMENT_OPERATORS.has(curAtom)) {
        AssignmentOperator({});
        AssignmentExpression(follow);
    }
    tb.treeNodeEnd();
}
void Parser::ConditionalExpression      (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::OrExpression               (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::AndExpression              (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::BitwiseOrExpression        (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::BitwiseXorExpression       (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::BitwiseAndExpression       (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::EqualityExpression         (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::RelationalExpression       (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::ShiftExpression            (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::AdditiveExpression         (const SymSet& follow) {
    tb.treeNodeStart("AdditiveExpression");
    MultiplicativeExpression(follow + ADDITIVE_OPERATORS);
    if (ADDITIVE_OPERATORS.has(curAtom)) {
        AdditiveOperator({});
        AdditiveExpression(follow);
    }
    tb.treeNodeEnd();
}
void Parser::MultiplicativeExpression   (const SymSet& follow) {
    tb.treeNodeStart("MultiplicativeExpression");
    UnaryExpression(follow + MULTIPLICATIVE_OPERATORS);
    if (MULTIPLICATIVE_OPERATORS.has(curAtom)) {
        MultiplicativeOperator({});
        UnaryExpression(follow);
    }
    tb.treeNodeEnd();
}

/*
UnaryExpression
    = MemberExpression
    | UnaryOperator UnaryExpression
    | '-' UnaryExpression
    | IncrementOperator MemberExpression // TODO
    | MemberExpression IncrementOperator
    | new Constructor // TODO
    | delete MemberExpression

// TODO
Constructor
    = this . ConstructorCall
    | ConstructorCall

ConstructorCall
    = ident
    | ident ( ArgumentListOpt )
    | ident . ConstructorCall

MemberExpression
    = PrimaryExpression
    | PrimaryExpression '.' MemberExpression
    | PrimaryExpression '[' Expression ']'
    | PrimaryExpression '(' ArgumentListOpt ')'

ArgumentListOpt
    = epsilon
    | ArgumentList

ArgumentList
    = AssignmentExpression
    | AssignmentExpression ',' ArgumentList

PrimaryExpression
    = '(' Expression ')'
    | ident
    | constant
    | false
    | true
    | null
    | this

AssignmentOperator
    = '='
    | '+='
    | '-='
    | '*='
    | '/='
    | '^='
    | '&='
    | '|='
    | '%='

EqualityOperator
    = '==' Expression
    | '!=' Expression

RelationalOperator
    = '<' Expression
    | '<=' Expression
    | '>' Expression
    | '>=' Expression

ShiftOperator
    = '<<'
    | '>>'
    | '>>>'

AdditiveOperator
    = '+'
    | '-'

MultiplicativeOperator
    = '*'
    | '/'

UnaryOperator
    = '--'
    = '++'
 * */
void Parser::UnaryExpression            (const SymSet& follow) {
    // TODO: implement properly
    tb.treeNodeStart("UnaryExpression");
    acceptConstant();
    tb.treeNodeEnd();
}
void Parser::Constructor                (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::ConstructorCall            (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::MemberExpression           (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::ArgumentListOpt            (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::ArgumentList               (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::PrimaryExpression          (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::AssignmentOperator         (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::EqualityOperator           (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::RelationalOperator         (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::ShiftOperator              (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
void Parser::AdditiveOperator           (const SymSet& follow) {
    tb.treeNodeStart("AdditiveOperator");
    vector<string> ops = ADDITIVE_OPERATORS_STRINGS;
    for (string o : ops) {
        if (isCurAtomKeyword(o)) {
            acceptKeyword(o);
            tb.treeNodeEnd();
            return;
        }
    }
    string msg = "Expected AdditiveOperator got: " + curAtom->getStr();
    errorsPositions.push_back(curAtom->getPos());
    errorsMessages.push_back(msg);
    tb.treeNodeEnd();
}
void Parser::MultiplicativeOperator     (const SymSet& follow) {
    tb.treeNodeStart("MultiplicativeOperator");
    vector<string> ops = MULTIPLICATIVE_OPERATORS_STRINGS;
    for (string o : ops) {
        if (isCurAtomKeyword(o)) {
            acceptKeyword(o);
            tb.treeNodeEnd();
            return;
        }
    }
    string msg = "Expected MultiplicativeOperator got: " + curAtom->getStr();
    errorsPositions.push_back(curAtom->getPos());
    errorsMessages.push_back(msg);
    tb.treeNodeEnd();
}
void Parser::UnaryOperator              (const SymSet& follow) {
    // TODO: implement
    assert(0);
}
