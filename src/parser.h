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
    bool isCurAtomConstant();
    bool isCurAtomSymbol();

    // parsing tree node methods
    void Element                    (const SymSet& follow);
    void FunctionDef                (const SymSet& follow);
    void ParameterList              (const SymSet& follow);
    void CompoundStatement          (const SymSet& follow);
    void Statements                 (const SymSet& follow);
    void Statement                  (const SymSet& follow);
    void Condition                  (const SymSet& follow);
    void VariableType               (const SymSet& follow);
    void VariablesOrExpression      (const SymSet& follow);
    void Variables                  (const SymSet& follow);
    void Variable                   (const SymSet& follow);
    void ExpressionOpt              (const SymSet& follow);


    void Expression                 (const SymSet& follow);
    void AssignmentExpression       (const SymSet& follow);
    void ConditionalExpression      (const SymSet& follow);
    void OrExpression               (const SymSet& follow);
    void AndExpression              (const SymSet& follow);
    void BitwiseOrExpression        (const SymSet& follow);
    void BitwiseXorExpression       (const SymSet& follow);
    void BitwiseAndExpression       (const SymSet& follow);
    void EqualityExpression         (const SymSet& follow);
    void RelationalExpression       (const SymSet& follow);
    void ShiftExpression            (const SymSet& follow);
    void AdditiveExpression         (const SymSet& follow);
    void MultiplicativeExpression   (const SymSet& follow);
    void UnaryExpression            (const SymSet& follow);
    void Constructor                (const SymSet& follow);
    void ConstructorCall            (const SymSet& follow);
    void MemberExpression           (const SymSet& follow);
    void ArgumentListOpt            (const SymSet& follow);
    void ArgumentList               (const SymSet& follow);
    void PrimaryExpression          (const SymSet& follow);
    void ArrayExpression            (const SymSet& follow);
    void AssignmentOperator         (const SymSet& follow);
    void EqualityOperator           (const SymSet& follow);
    void RelationalOperator         (const SymSet& follow);
    void ShiftOperator              (const SymSet& follow);
    void AdditiveOperator           (const SymSet& follow);
    void MultiplicativeOperator     (const SymSet& follow);
    void UnaryOperator              (const SymSet& follow);

    vector<std::pair<unsigned,unsigned>> errorsPositions;
    vector<string> errorsMessages;

    TreeBuilder tb;

    bool canParse;

    void notExpectedAtomError(const Atom* a) {
        errorsPositions.push_back(a->getPos());
        errorsMessages.push_back(
                "Unexpected atom: " + a->getStr());
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
    std::shared_ptr<Node> getTreeRoot() { return tb.getTreeRoot(); }

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
