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

const static SymSet EXPRESSION_FIRST =
    SymSet({"CONSTANT", "SYMBOL", "("});
const static SymSet STATEMENT_FIRST =
    SymSet({";", "var", "const", "let"}) + EXPRESSION_FIRST;
const static SymSet ELEMENT_FIRST =
    SymSet({"function"}) + STATEMENT_FIRST;

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

bool Parser::isCurAtomConstant() {
    return dynamic_cast<AtomConstant*>(curAtom);
}

bool Parser::isCurAtomSymbol() {
    return dynamic_cast<AtomSymbol*>(curAtom);
}


/*
Program
    = { Element }
    | epsilon
*/
void Parser::Program() {
    tb.treeNodeStart("Program");
    SymSet first = ELEMENT_FIRST + SymSet({"$"});

    while (!isCurAtomKeyword("$")) {
        Element(first);
    }

    tb.treeNodeEnd();
}

/*
Element
    = function Identifier '(' ParameterListOpt ')' CompoundStatement
    | Statement

ParameterListOpt
    = empty
    | ParameterList

ParameterList
    = ident
    | ident , ParameterList

CompoundStatement
    = '{' Statements '}'

Statements
    = empty
    | Statement Statements

Statement
    ;
    if Condition Statement
    if Condition Statement else Statement
    while Condition Statement
    ForParen ';' ExpressionOpt ';' ExpressionOpt ')' Statement
    ForBegin ';' ExpressionOpt ';' ExpressionOpt ')' Statement
    ForBegin in Expression ')' Statement
    break ';'
    continue ';'
    with '(' Expression ')' Statement
    return ExpressionOpt ';'
    CompoundStatement
    VariablesOrExpression ';'

Condition
    = '(' Expression ')'

ForParen
    = for '('

ForBegin
    = ForParen VariablesOrExpression

VariableType
    = var
    | let
    | const

VariablesOrExpression
    = VariableType Variables
    | Expression

Variables
    = Variable
    | Variable , Variables

Variable
    = ident
    | ident = AssignmentExpression

ExpressionOpt
    = empty
    | Expression
*/
// TODO: implement whole
void Parser::Element(const SymSet& follow) {
    SymSet first = ELEMENT_FIRST;
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Element");

    if (isCurAtomKeyword("function")) {
        // TODO
        assert(0);
    } else {
        Statement(follow);
    }

    tb.treeNodeEnd();
}

void Parser::ParameterListOpt           (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::ParameterList              (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::CompoundStatement          (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::Statements                 (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::Statement                  (const SymSet& follow){
    SymSet first = STATEMENT_FIRST;
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Statement");

    // TODO: implement rest
    if (isCurAtomKeyword(";")) {
        acceptKeyword(";");
    } else {
        VariablesOrExpression(follow + SymSet{";"});
        acceptKeyword(";");
    }

    tb.treeNodeEnd();
}
void Parser::Condition                  (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::ForParen                   (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::ForBegin                   (const SymSet& follow){
    // TODO: implement
    assert(0);
}
void Parser::VariableType               (const SymSet& follow){
    SymSet first = {"var", "let", "const"};
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("VariableType");

    if (isCurAtomKeyword("var")) {
        acceptKeyword("var");
    } else if (isCurAtomKeyword("let")) {
        acceptKeyword("let");
    } else if (isCurAtomKeyword("const")) {
        acceptKeyword("const");
    }

    tb.treeNodeEnd();
}
void Parser::VariablesOrExpression      (const SymSet& follow){
    SymSet first = SymSet{"var", "let", "const"} + EXPRESSION_FIRST;
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("VariablesOrExpression");

    if (EXPRESSION_FIRST.has(curAtom)) {
        Expression(follow);
    } else {
        VariableType(follow + SymSet{"SYMBOL"});
        Variables(follow);
    }

    tb.treeNodeEnd();
}
void Parser::Variables                  (const SymSet& follow){
    SymSet first = {"SYMBOL"};
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Variables");

    Variable(follow + SymSet{","});
    while (isCurAtomKeyword(",")) {
        acceptKeyword(",");
        Variable(follow + SymSet{","});
    }

    tb.treeNodeEnd();
}
void Parser::Variable                   (const SymSet& follow){
    SymSet first = {"SYMBOL"};
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Variable");

    acceptSymbol();
    if (isCurAtomKeyword("=")) {
        acceptKeyword("=");
        AssignmentExpression(follow);
    }

    tb.treeNodeEnd();
}
void Parser::ExpressionOpt              (const SymSet& follow){
    // TODO: implement
    assert(0);
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
    SymSet first = EXPRESSION_FIRST;
    Synchronize s(this, first, follow);
    if (!canParse) return;
    tb.treeNodeStart("Expression");

    AssignmentExpression(follow + SymSet{","});
    if (isCurAtomKeyword(",")) {
        acceptKeyword(",");
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
        MultiplicativeExpression(follow);
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
    MemberExpression(follow);
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
    tb.treeNodeStart("MemberExpression");

    PrimaryExpression(follow + SymSet{".", "[", "("});
    if (isCurAtomKeyword(".")) {
        acceptKeyword(".");
        MemberExpression(follow);
    } else if (isCurAtomKeyword("[")) {
        acceptKeyword("[");
        Expression(follow + SymSet{"]"});
        acceptKeyword("]");
    } else if (isCurAtomKeyword("(")) {
        acceptKeyword("(");
        ArgumentListOpt(follow + SymSet{")"});
        acceptKeyword(")");
    }

    tb.treeNodeEnd();
}
void Parser::ArgumentListOpt            (const SymSet& follow) {
    tb.treeNodeStart("ArgumentListOpt");
    if (!isCurAtomKeyword(")"))
        ArgumentList(follow);
    tb.treeNodeEnd();
}
void Parser::ArgumentList               (const SymSet& follow) {
    tb.treeNodeStart("ArgumentList");
    AssignmentExpression(follow + SymSet{","});
    while (isCurAtomKeyword(",")) {
        acceptKeyword(",");
        AssignmentExpression(follow + SymSet{","});
    }
    tb.treeNodeEnd();
}
void Parser::PrimaryExpression          (const SymSet& follow) {
    tb.treeNodeStart("PrimaryExpression");

    if (isCurAtomKeyword("(")) {
        acceptKeyword("(");
        Expression(follow + SymSet{")"});
        acceptKeyword(")");
    } else if (isCurAtomConstant()) {
        acceptConstant();
    } else if (isCurAtomSymbol()) {
        acceptSymbol();
    } else {
        // TODO: false true null this
    }

    tb.treeNodeEnd();
}
void Parser::AssignmentOperator         (const SymSet& follow) {
    tb.treeNodeStart("AssignmentOperator");
    vector<string> ops = ASSIGNMENT_OPERATORS_STRINGS;
    for (string o : ops) {
        if (isCurAtomKeyword(o)) {
            acceptKeyword(o);
            tb.treeNodeEnd();
            return;
        }
    }
    string msg = "Expected AssignmentOperator got: " + curAtom->getStr();
    errorsPositions.push_back(curAtom->getPos());
    errorsMessages.push_back(msg);
    tb.treeNodeEnd();
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
