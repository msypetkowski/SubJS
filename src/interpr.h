#pragma once

#include "atom.h"
#include "tree.h"

#include <map>
#include <iostream> // TODO: remove

class Context;

class Value {
    public: // TODO: remove
    string type;
    float floatData;
    int intData;
    string stringData;
    std::vector<Value> arrayData;
    Context* context;
public:
    Value():type("undefined"){};
    Value(Context*, Atom*);
    Value(Context* c):type("undefined"),context(c){};

    Value op(string op, Value);
    Value member(Value); // operator "."
    Value call(std::vector<Value>&);

    Value operator[](const Value&);

    string getRepr();
};

class Context {
    std::map<string, Value> variables;
public:
    void addVariable(string varName, Value value) {
        variables[varName] = value;
    }
    Value getValue(string name) {
        return variables[name];
    };
    //Value getValue(string name) { return Value(this); };
};

class Interpreter {
    std::shared_ptr<Node> root;

    Context context;

    void Program                    (Node *n);

    void Element                    (Node* n);
    void ParameterListOpt           (Node* n);
    void ParameterList              (Node* n);
    void CompoundStatement          (Node* n);
    void Statements                 (Node* n);
    void Statement                  (Node* n);
    void Condition                  (Node* n);
    void ForParen                   (Node* n);
    void ForBegin                   (Node* n);
    void VariableType               (Node* n);
    void VariablesOrExpression      (Node* n);
    void Variables                  (Node* n);
    void Variable                   (Node* n);
    void ExpressionOpt              (Node* n);

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
