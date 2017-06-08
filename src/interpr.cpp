#include "interpr.h"
#include "builtins.h"

#include <cassert>
#include <iostream>

void Interpreter::run() {
    assert(root->subNodes.size()==1);
    auto program = root->subNodes[0];

    // builtin variables
    context.addVariable("print", Value(Val(new BuiltInPrint(&context))));
    context.addVariable("eval", Value(Val(new BuiltInEval(&context))));

    Program(program.get());
    assert(program->name == "Program");
}

void Interpreter::Program(Node* program) {
    assert(program->name == "Program");
    for(auto element : program->subNodes) {
        assert(element->name == "Element");
        Element(element.get());
    }
}

void Interpreter::Element(Node *n) {
    assert(n->name == "Element");
    if (n->subNodes[0]->name == "Statement") {
        Statement(n->subNodes[0].get());
    } else {
        assert(0);
    }
}

Value Interpreter::FunctionDef(Node *n) {
    assert(n->name == "FunctionDef");
    assert(n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "function");

    AtomSymbol* functionSymbol =
        dynamic_cast<AtomSymbol*>(n->subNodes[1]->data);
    assert(functionSymbol);
    auto params = ParameterList(n->subNodes[3].get());
    return Value(&context, functionSymbol, params, n->subNodes[5].get());
}
vector<AtomSymbol*> Interpreter::ParameterList              (Node* n) {
    assert(n->name == "ParameterList");
    vector<AtomSymbol*> ret;
    for (unsigned i=0; i<n->subNodes.size(); i+=2) {
        AtomSymbol* s = dynamic_cast<AtomSymbol*>(
                n->subNodes[i]->data);
        assert(s);
        ret.push_back(s);
    }
    return ret;
}
Value Interpreter::CompoundStatement          (Node* n) {
    retVal = Value(&context);
    assert(n->name == "CompoundStatement");
    Statements(n->subNodes[1].get());
    return retVal;
}
void Interpreter::Statements                 (Node* n) {
    assert(n->name == "Statements");
    Statement(n->subNodes[0].get());
    if (n->subNodes.size() == 2) {
        Statements(n->subNodes[1].get());
    }
}
void Interpreter::Statement                  (Node* n) {
    assert(n->name == "Statement");
    if (n->subNodes[0]->name == "VariablesOrExpression") {
        VariablesOrExpression(n->subNodes[0].get());
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == ";"){
        return;
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "if"){
        if (n->subNodes.size() == 3) {
            Value cond = Condition(n->subNodes[1].get());
            if (cond.cond())
                Statement(n->subNodes[2].get());
        } else if (n->subNodes.size() == 5) {
            Value cond = Condition(n->subNodes[1].get());
            if (cond.cond())
                Statement(n->subNodes[2].get());
            else
                Statement(n->subNodes[4].get());
        } else { 
            assert(0);
        }
    } else  if (n->subNodes[0]->name == "CompoundStatement") {
        CompoundStatement(n->subNodes[0].get());
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "return"){
        retVal = ExpressionOpt(n->subNodes[1].get());
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "while"){
        Value cond = Condition(n->subNodes[1].get());
        while(cond.cond()) {
                Statement(n->subNodes[2].get());
                cond = Condition(n->subNodes[1].get());
        }
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "for"){
        // for '(' VariablesOrExpression ';' ExpressionOpt ';' ExpressionOpt ')' Statement
        // for '(' ';' ExpressionOpt ';' ExpressionOpt ')' Statement
        if (n->subNodes[2]->name == "VariablesOrExpression") {
            VariablesOrExpression(n->subNodes[2].get());
            for(;ExpressionOpt(n->subNodes[4].get()).cond();
                 ExpressionOpt(n->subNodes[6].get())) {
                Statement(n->subNodes[8].get());
            }
        } else {
            for(;ExpressionOpt(n->subNodes[3].get()).cond();
                 ExpressionOpt(n->subNodes[5].get())) {
                Statement(n->subNodes[7].get());
            }
        }
    } else {
        // TODO: implement
        context.dump();
    }
}
Value Interpreter::Condition                  (Node* n) {
    assert(n->name == "Condition");
    assert(n->subNodes.size() == 3);
    return Expression(n->subNodes[1].get());
}
void Interpreter::ForParen                   (Node* n) {
    // TODO: implement
    context.dump();
}
void Interpreter::ForBegin                   (Node* n) {
    // TODO: implement
    context.dump();
}
void Interpreter::VariableType               (Node* n) {
    // TODO: implement
    context.dump();
}
void Interpreter::VariablesOrExpression      (Node* n) {
    assert(n->name == "VariablesOrExpression");
    if (n->subNodes[0]->name == "Expression") {
        Expression(n->subNodes[0].get());
    } else {
        Variables(n->subNodes[1].get());
    }
}
void Interpreter::Variables                  (Node* n) {
    assert(n->name == "Variables");
    for (unsigned i=0; i<n->subNodes.size(); i+=2) {
        Variable(n->subNodes[i].get());
    }
}
void Interpreter::Variable                   (Node* n) {
    assert(n->name == "Variable");
    if (n->subNodes.size() == 3) {
        string varname = n->subNodes[0]->data->getRepr();
        Value v = AssignmentExpression(n->subNodes[2].get());
        context.addVariable(varname, v);
    }
}
Value Interpreter::ExpressionOpt              (Node* n) {
    assert(n->name == "ExpressionOpt");
    if (n->subNodes.size() == 1) {
        return Expression(n->subNodes[0].get());
    } else {
        assert(0);
    }
    return Value();
}

Value Interpreter::Expression                 (Node* n) {
    assert(n->name == "Expression");
    Value v = AssignmentExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3 ) {
        return Expression(n->subNodes[2].get());
    }
    return v;
}
Value Interpreter::AssignmentExpression       (Node* n) {
    assert(n->name == "AssignmentExpression");
    // TODO: ConditionalExpression here
    Value v = EqualityExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3){
        string op = n->subNodes[1]->subNodes[0]->data->getRepr();
        return v.op(op, AssignmentExpression(n->subNodes[2].get()));
    }
    return v;
}
// Value Interpreter::ConditionalExpression      (Node* n) {
//     // TODO: implement
//     assert(0);
// }
// Value Interpreter::OrExpression               (Node* n) {
//     // TODO: implement
//     assert(0);
// }
// Value Interpreter::AndExpression              (Node* n) {
//     // TODO: implement
//     assert(0);
// }
// Value Interpreter::BitwiseOrExpression        (Node* n);
// Value Interpreter::BitwiseXorExpression       (Node* n);
// Value Interpreter::BitwiseAndExpression       (Node* n);
Value Interpreter::EqualityExpression         (Node* n) {
    assert(n->name == "EqualityExpression");
    Value v = RelationalExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3){
        string op = n->subNodes[1]->subNodes[0]->data->getRepr();
        return v.op(op, EqualityExpression(n->subNodes[2].get()));
    }
    assert(n->subNodes.size() == 1);
    return v;
}
Value Interpreter::RelationalExpression       (Node* n) {
    assert(n->name == "RelationalExpression");
    Value v = AdditiveExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3){
        string op = n->subNodes[1]->subNodes[0]->data->getRepr();
        return v.op(op, RelationalExpression(n->subNodes[2].get()));
    }
    return v;
}
// Value Interpreter::ShiftExpression            (Node* n);
Value Interpreter::AdditiveExpression         (Node* n) {
    assert(n->name == "AdditiveExpression");
    Value v = MultiplicativeExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3){
        string op = n->subNodes[1]->subNodes[0]->data->getRepr();
        return v.op(op, AdditiveExpression(n->subNodes[2].get()));
    }
    return v;
}
Value Interpreter::MultiplicativeExpression   (Node* n) {
    assert(n->name == "MultiplicativeExpression");
    Value v = UnaryExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3){
        string op = n->subNodes[1]->subNodes[0]->data->getRepr();
        return v.op(op, MultiplicativeExpression(n->subNodes[2].get()));
    }
    return v;
}
Value Interpreter::UnaryExpression            (Node* n) {
    assert(n->name == "UnaryExpression");
    if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == "-"){
        return UnaryExpression(n->subNodes[1].get()).unaryOp("-");
    } else {
        if (n->subNodes.size() == 2) {
            assert(n->subNodes[1]->data->getRepr() == "++");
            return MemberExpression(n->subNodes[0].get()).unaryOp("++");
        } else {
            return MemberExpression(n->subNodes[0].get());
        }
    }
}
// Value Interpreter::Constructor                (Node* n);
// Value Interpreter::ConstructorCall            (Node* n);
Value Interpreter::MemberExpression           (Node* n) {
    assert(n->name == "MemberExpression");
    Value v = PrimaryExpression(n->subNodes[0].get());

    unsigned cur = 1;
    while(cur < n->subNodes.size()) {
        if (n->subNodes[cur]->data->getRepr() == ".") {
            v = v.member(PrimaryExpression(n->subNodes[cur+1].get()));
            cur += 2;
        } else if (n->subNodes[cur]->data->getRepr() == "(") {
            std::vector<Value> args=ArgumentListOpt(n->subNodes[cur+1].get());
            v = v.call(args);
            cur += 3;
        } else if (n->subNodes[cur]->data->getRepr() == "[") {
            Value key = Expression(n->subNodes[cur+1].get());
            v = v[key];
            cur += 3;
        } else {
            assert(0);
        }
    }
    return v;
}
std::vector<Value> Interpreter::ArgumentListOpt            (Node* n) {
    assert(n->name == "ArgumentListOpt");
    if (n->subNodes.size()) {
        return ArgumentList(n->subNodes[0].get());
    }
    return {};
}
std::vector<Value> Interpreter::ArgumentList               (Node* n) {
    assert(n->name == "ArgumentList");
    std::vector<Value> ret;
    for (unsigned i=0; i<n->subNodes.size(); i+=2) {
        ret.push_back(AssignmentExpression(n->subNodes[i].get()));
    }
    return ret;
}
Value Interpreter::PrimaryExpression          (Node* n) {
    assert(n->name == "PrimaryExpression");
    if (n->subNodes.size() == 3) {
        string op = n->subNodes[0]->data->getRepr();
        if (op == "(") {
            return Expression(n->subNodes[1].get());
        } else if (op == "[") {
            return ArrayExpression(n->subNodes[1].get());
        }
    } else if (n->subNodes[0]->name == "FunctionDef") {
        return FunctionDef(n->subNodes[0].get());
    }
    assert(n->subNodes.size() == 1);
    return Value(&context, n->subNodes[0]->data);
}
Value Interpreter::ArrayExpression             (Node* n) {
    assert(n->name == "ArrayExpression");
    unsigned cur = 0;
    std::vector<Value> arrayElems;
    bool lastWasComma = true;
    while(cur < n->subNodes.size()) {
        auto arrayElem = n->subNodes[cur];
        if (arrayElem->name == "AssignmentExpression") {
            arrayElems.push_back(AssignmentExpression(arrayElem.get()));
            lastWasComma = false;
        } else if (arrayElem->data) {
            if (lastWasComma)
                arrayElems.push_back(Value(&context));
            lastWasComma = true;
        }
        ++cur;
    }
    Value ret(&context, arrayElems);
    // ret.type = "array";
    // ret.arrayData = arrayElems;
    return ret;
}
// Value Interpreter::AssignmentOperator         (Node* n);
// Value Interpreter::EqualityOperator           (Node* n);
// Value Interpreter::RelationalOperator         (Node* n);
// Value Interpreter::ShiftOperator              (Node* n);
// Value Interpreter::AdditiveOperator           (Node* n);
// Value Interpreter::MultiplicativeOperator     (Node* n);
// Value Interpreter::UnaryOperator              (Node* n);
