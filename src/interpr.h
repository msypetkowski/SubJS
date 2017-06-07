#pragma once

#include "atom.h"
#include "tree.h"
#include "value.h"
#include "context.h"

class Interpreter {
    std::shared_ptr<Node> root;

    Context context;

    void Program                    (Node *n);

    void Element                    (Node* n);
    Value FunctionDef               (Node* n);
    vector<AtomSymbol*> ParameterList              (Node* n);
    void CompoundStatement          (Node* n);
    void Statements                 (Node* n);
    void Statement                  (Node* n);
    Value Condition                 (Node* n);
    void ForParen                   (Node* n);
    void ForBegin                   (Node* n);
    void VariableType               (Node* n);
    void VariablesOrExpression      (Node* n);
    void Variables                  (Node* n);
    void Variable                   (Node* n);
    Value ExpressionOpt              (Node* n);

    Value Expression                  (Node* n);
    Value AssignmentExpression        (Node* n);
    Value ConditionalExpression       (Node* n);
    Value OrExpression                (Node* n);
    Value AndExpression               (Node* n);
    Value BitwiseOrExpression         (Node* n);
    Value BitwiseXorExpression        (Node* n);
    Value BitwiseAndExpression        (Node* n);
    Value EqualityExpression          (Node* n);
    Value RelationalExpression        (Node* n);
    Value ShiftExpression             (Node* n);
    Value AdditiveExpression          (Node* n);
    Value MultiplicativeExpression    (Node* n);
    Value UnaryExpression             (Node* n);
    Value Constructor                 (Node* n);
    Value ConstructorCall             (Node* n);
    Value MemberExpression            (Node* n);
    std::vector<Value> ArgumentListOpt(Node* n);
    std::vector<Value> ArgumentList   (Node* n);
    Value PrimaryExpression           (Node* n);
    Value ArrayExpression             (Node* n);
    Value AssignmentOperator          (Node* n);
    Value EqualityOperator            (Node* n);
    Value RelationalOperator          (Node* n);
    Value ShiftOperator               (Node* n);
    Value AdditiveOperator            (Node* n);
    Value MultiplicativeOperator      (Node* n);
    Value UnaryOperator               (Node* n);


public:
    Interpreter(std::shared_ptr<Node> n):root(n) {}

    void run();
};
