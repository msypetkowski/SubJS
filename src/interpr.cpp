#include "interpr.h"

#include <boost/lexical_cast.hpp>

#include <cassert>
#include <iostream>

static string parseStringLiteral(string lit) {
    // TODO: do it properly
    unsigned cur=0;
    string ret;
    while(cur < lit.size()) {
        if (lit[cur]=='\\') {
            string str=string("0x") + lit[cur+2] + lit[cur+3];
            ret+=(char) std::stoul(str, nullptr, 16);
            cur+=4;
        } else {
            ret+=lit[cur];
            ++cur;
        }
    }
    return ret;
}

Value::Value(Context* c, Atom* a):type("undefined"), context(c) {
    AtomConstant* ac;
    AtomSymbol* as;
    if ((ac=dynamic_cast<AtomConstant*>(a))) {
        if (ac->type == AtomConstant::Type::String) {
            type = "string";
            stringData = parseStringLiteral(ac->getRepr());
        } else if (ac->type == AtomConstant::Type::Integer) {
            type = "int";
            intData = boost::lexical_cast<int>((ac->getRepr()));
        } else if (ac->type == AtomConstant::Type::Float) {
            type = "float";
            intData = boost::lexical_cast<float>((ac->getRepr()));
        } else {
            assert(0);
        }
    } else if ((as=dynamic_cast<AtomSymbol*>(a))) {
        type = "symbol";
        stringData = as->getRepr();
    } else {
        // TODO: implement
        assert(0);
    }
}

Value Value::op(string opr, Value v) {
    Value ret(context);
    if (opr == "=") {
        if (type != "symbol") {
            // TODO: error handling
            assert(0);
        }
        if (v.type == "symbol") {
            // TODO: referention
            Value lVal = context->getValue(v.stringData);
            context->addVariable(stringData, lVal);
            ret = lVal;
        } else {
            context->addVariable(stringData, v);
            ret = v;
        }
    } else if (opr == "+=") {
        ret = op("=", op("+",v));
    } else if (type == "string" && v.type == "string") {
        ret.type = "string";
        if (opr == "+") {
            ret.stringData = stringData + v.stringData;
        } else if (opr == "-") {
            // TODO; error handling
            assert(0);
        } else {
            // TODO: unknown operator
            assert(0);
        }
    } else if (type == "int" && v.type == "int") {
        ret.type = "int";
        if (opr == "+") {
            ret.intData = intData + v.intData;
        } else if (opr == "-") {
            ret.intData = intData - v.intData;
        } else if (opr == "*") {
            ret.intData = intData * v.intData;
        } else {
            // TODO: unknown operator
            assert(0);
        }
    } else if (type == "symbol") {
        Value myVal = context->getValue(stringData);
        ret = myVal.op(opr, v);
    } else if (v.type == "symbol") {
        Value vVal = context->getValue(v.stringData);
        ret = op(opr, vVal);
    } else {
        // TODO; implement
        assert(0);
    }
    return ret;
}

Value Value::member(Value) {
    // TODO; implement
    assert(0);
    return Value(context);
}
Value Value::call(std::vector<Value>& args) {
    if (type == "symbol" && stringData == "print") {
        for(unsigned i=0; i<args.size(); ++i) {
            if (i == args.size() - 1)
                std::cout << args[i].getRepr();
            else
                std::cout << args[i].getRepr() << " ";
        }
        std::cout<<std::endl;
        Value ret(context);
        ret.type="undefined";
        return ret;
    } else {
        // TODO; implement
        assert(0);
    }
    assert(0);
    return Value(context);
}

string Value::getRepr() {
    if (type == "int")
        return std::to_string(intData);
    else if (type == "float")
        return std::to_string(floatData);
    else if (type == "string")
        return stringData;
    else if (type == "symbol")
        return context->getValue(stringData).getRepr();
    else if (type == "undefined")
        return "";
    else if (type == "array") {
        string ret;
        for(auto v:arrayData) {
            ret += v.getRepr() + ",";
        }
        ret = ret.substr(0, ret.size()-1);
        return ret;
    }
    assert(0);
    return "";
}

void Interpreter::run() {
    assert(root->subNodes.size()==1);
    auto program = root->subNodes[0];
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

    assert(n->subNodes.size()==1);

    if (n->subNodes[0]->name == "Statement") {
        Statement(n->subNodes[0].get());
    } else {
        //TODO: implement function
        assert(0);
    }
}

void Interpreter::ParameterListOpt           (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::ParameterList              (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::CompoundStatement          (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::Statements                 (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::Statement                  (Node* n) {
    assert(n->name == "Statement");
    if (n->subNodes[0]->name == "VariablesOrExpression") {
        VariablesOrExpression(n->subNodes[0].get());
    } else if (n->subNodes[0]->data != nullptr
            && n->subNodes[0]->data->getRepr() == ";"){
        return;
    } else {
        // TODO: implement
        assert(0);
    }
}
void Interpreter::Condition                  (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::ForParen                   (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::ForBegin                   (Node* n) {
    // TODO: implement
    assert(0);
}
void Interpreter::VariableType               (Node* n) {
    // TODO: implement
    assert(0);
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
// void Interpreter::ExpressionOpt              (Node* n) {
//     // TODO: implement
//     assert(0);
// }
//
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
    Value v = AdditiveExpression(n->subNodes[0].get());
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
// Value Interpreter::EqualityExpression         (Node* n);
// Value Interpreter::RelationalExpression       (Node* n);
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
    // TODO: implement rest
    return MemberExpression(n->subNodes[0].get());
}
// Value Interpreter::Constructor                (Node* n);
// Value Interpreter::ConstructorCall            (Node* n);
Value Interpreter::MemberExpression           (Node* n) {
    assert(n->name == "MemberExpression");
    Value v = PrimaryExpression(n->subNodes[0].get());
    if (n->subNodes.size() == 3) {
        string op = n->subNodes[1]->data->getRepr();
        assert(op==".");
        return v.member(MemberExpression(n->subNodes[2].get()));
    }
    if (n->subNodes.size() == 4) {
        string op = n->subNodes[1]->data->getRepr();
        if (op == "(") {
            std::vector<Value> args=ArgumentListOpt(n->subNodes[2].get());
            v.call(args);
        } else if (op == "[") {
            // TODO: implement
            assert(0);
        } else assert(0);
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
    }
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
    Value ret(&context);
    ret.type = "array";
    ret.arrayData = arrayElems;
    return ret;
}
// Value Interpreter::AssignmentOperator         (Node* n);
// Value Interpreter::EqualityOperator           (Node* n);
// Value Interpreter::RelationalOperator         (Node* n);
// Value Interpreter::ShiftOperator              (Node* n);
// Value Interpreter::AdditiveOperator           (Node* n);
// Value Interpreter::MultiplicativeOperator     (Node* n);
// Value Interpreter::UnaryOperator              (Node* n);
