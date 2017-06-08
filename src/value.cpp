#include "value.h"
#include "atom.h"
#include "interpr.h"
#include "builtins.h"

#include <boost/lexical_cast.hpp>


static string parseStringLiteral(string lit) {
    // TODO: test it
    unsigned cur=0;
    string ret;
    while(cur < lit.size()) {
        if (lit[cur]=='\\') {
            if (lit[cur+1] == 'x') {
                string str=string("0x") + lit[cur+2] + lit[cur+3];
                ret+=(char) std::stoul(str, nullptr, 16);
                cur+=4;
            } else if (lit[cur+1] == 'n'){
                ret+='\n';
                cur+=2;
            } else if (lit[cur+1] == 'r'){
                ret+='\r';
                cur+=2;
            } else if (lit[cur+1] == '\\'){
                ret+='\\';
                cur+=2;
            } else {
                std::cout<<lit[cur]<<std::endl;
                assert(0);
            }
        } else {
            ret+=lit[cur];
            ++cur;
        }
    }
    return ret;
}

Value::Value():context(nullptr) {
    data = Val(new ValueUndefined(nullptr));
}

Value::Value(Context* c, Atom* atom):context(c) {
    AtomConstant* ac;
    AtomSymbol* as;
    if ((ac=dynamic_cast<AtomConstant*>(atom))) {
        if (ac->type == AtomConstant::Type::String) {
            string stringData = parseStringLiteral(ac->getRepr());
            data = Val(new ValueString(context, stringData));
        } else if (ac->type == AtomConstant::Type::Integer) {
            int intData = boost::lexical_cast<int>((ac->getRepr()));
            data = Val(new ValueInteger(context, intData));
        } else if (ac->type == AtomConstant::Type::Float) {
            float floatData = boost::lexical_cast<float>((ac->getRepr()));
            data = Val(new ValueFloat(context, floatData));
        } else {
            context->dump();
        }
    } else if ((as=dynamic_cast<AtomSymbol*>(atom))) {
        string str = as->getRepr();
        data = Val(new ValueSymbol(context, str));
    } else {
        // TODO: implement
        context->dump();
    }
}

Value::Value(Context* c):context(c) {
    data = Val(new ValueUndefined(c));
}

Value::Value(Val v)
    :context(v->getContext()), data(v) {
}

Value::Value(Context* c, vector<Value> v)
    :context(c) {
    std::vector<Val> param;
    for (auto a : v)
        param.push_back(a.data);
    data = Val(new ValueArray(c, param));
}
Value::Value(Context* c, AtomSymbol* s, vector<AtomSymbol*> params, Node* node)
    :context(c) {
    vector<string> paramsNames;
    for(auto sym : params) {
        paramsNames.push_back(sym->getRepr());
    }
    data = Val(new ValueFunction(context, s->getRepr(), paramsNames, node));
    context->addVariable(s->getRepr(), *this);
}

Value Value::op(string opr, Value v) {
    return Value(data->op(opr, v.data));
}

Value Value::unaryOp(string opr){
    return Value(data->unaryOp(opr));
}
Value Value::member(Value v) {
    return Value(data->member(v.data));
}
Value Value::call(std::vector<Value>& v) {
    std::vector<Val> param;
    for (auto a : v)
        param.push_back(a.data);
    return Value(data->call(param));
}
Value Value::operator[](const Value& v) {
    return Value((*data)[v.data]);
}
string Value::getRepr() {
    return data->getRepr();
}
bool Value::cond()const {
    return data->cond();
}


Val ValueBase::op(string, Val) {
    context->dump();
    return Val(new ValueUndefined(context));
}
Val ValueBase::unaryOp(string) {
    context->dump();
    return Val(new ValueUndefined(context));
}
Val ValueBase::member(Val) {
    context->dump();
    return Val(new ValueUndefined(context));
}
Val ValueBase::call(std::vector<Val>&) {
    context->dump();
    return Val(new ValueUndefined(context));
}
Val ValueBase::operator[](const Val&) {
    context->dump();
    return Val(new ValueUndefined(context));
}
string ValueBase::getRepr() {
    context->dump();
    return "";
}

Val ValueSymbol::op(string opr,Val v) {
    if (opr == "=") {
        if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
            Val val = context->getValue(vs->getData()).get();
            return op(opr, val);
        } else {
            context->addVariable(data, v);
            return v;
        }
    } else if (opr == "+=") {
        return op("=", op("+", v));
    } else if (opr == "-=") {
        return op("=", op("-", v));
    } else if (opr == "*=") {
        return op("=", op("*", v));
    } else if (opr == "/=") {
        return op("=", op("/", v));
    } else {
        Value val = context->getValue(data);
        return val.get()->op(opr, v);
    }
}

Val ValueSymbol::unaryOp(string opr) {
    if(opr == "-") {
        Value val = context->getValue(data);
        return val.get()->unaryOp(opr);
    } else {
        context->dump();
        return Val(new ValueUndefined(context));
    }
}

Val ValueSymbol::call(std::vector<Val>& v) {
    Value val = context->getValue(data);
    return val.get()->call(v);
}

Val ValueSymbol::operator[](const Val& v) {
    Val val = context->getValue(data).get();
    return (*val)[v];
}

string ValueSymbol::getRepr() {
    Value val = context->getValue(data);
    return val.get()->getRepr();
}

Val ValueFunction::call(std::vector<Val>& args) {
    assert(args.size() == params.size());
    // TODO: implement properly
    if (args.empty()) {
        return context->callNode(node).get();
        //return Val(new ValueUndefined(context));
    } else {
        return args[0];
    }
}

string ValueFunction::getRepr() {
    return  "function " + name + "() {\n    [some code]\n}";
}

Val ValueUndefined::op(string, Val) {
    return Val(new ValueUndefined(context));
}
Val ValueArray::operator[](const Val& v) {
    if (ValueInteger* vi = dynamic_cast<ValueInteger*>(v.get())) {
        if (vi->getData() >= 0 && (unsigned)vi->getData() < data.size()) {
            return data[vi->getData()];
        } else {
            return Val(new ValueUndefined(context));
        }
    } else if (ValueString* vs = dynamic_cast<ValueString*>(v.get())) {
        if (vs->getData() == "constructor") {
            return Val(new BuiltInArray(context));
        } else if (vs->getData() == "reverse") {
            return Val(new BuiltInReverse(context, data));
        } else if (vs->getData() == "join") {
            return Val(new BuiltInJoin(context, data));
        } else {
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vsym = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vsym->getData()).get();
        return (*this)[val];
    } else {
        return Val(new ValueUndefined(context));
    }
}

string ValueArray::getRepr() {
    string ret;
    for(auto v:data) {
        if (dynamic_cast<ValueUndefined*>(v.get()))
            ret += ",";
        else
            ret += v->getRepr() + ",";
    }
    ret = ret.substr(0, ret.size()-1);
    return ret;
}


Val ValueInteger::op(string opr, Val v) {
    if (ValueInteger* vi = dynamic_cast<ValueInteger*>(v.get())) {
        if (opr == "+") {
            return Val(new ValueInteger(context, data + vi->data));
        } else if (opr == "-") {
            return Val(new ValueInteger(context, data - vi->data));
        } else if (opr == "*") {
            return Val(new ValueInteger(context, data * vi->data));
        } else if (opr == "==") {
            return Val(new ValueBoolean(context, data == vi->data));
        } else if (opr == "!=") {
            return Val(new ValueBoolean(context, data != vi->data));
        } else if (opr == "<") {
            return Val(new ValueBoolean(context, data < vi->data));
        } else if (opr == ">") {
            return Val(new ValueBoolean(context, data > vi->data));
        } else if (opr == ">=") {
            return Val(new ValueBoolean(context, data >= vi->data));
        } else if (opr == "<=") {
            return Val(new ValueBoolean(context, data <= vi->data));
        } else {
            context->dump();
            return Val(new ValueUndefined(context));
        }
        
    } else if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vs->getData()).get();
        return op(opr, val);
    } else {
        context->dump();
        return Val(new ValueUndefined(context));
    }
}
Val ValueInteger::unaryOp(string opr) {
    if (opr == "-") {
        return Val(new ValueInteger(context, -data));
    } else {
        context->dump();
        return Val(new ValueUndefined(context));
    }
}

Val ValueInteger::operator[](const Val&) {
    // TODO: "constructor" etc.
    return Val(new ValueUndefined(context));
}

string ValueInteger::getRepr() {
    return std::to_string(data);
}

Val ValueString::op(string opr, Val v) {
    if (ValueString* va = dynamic_cast<ValueString*>(v.get())) {
        if (opr == "+") {
            return Val(new ValueString(context, data + va->data));
        } else {
            context->dump();
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vs->getData()).get();
        return op(opr, val);
    } else {
        context->dump();
        return Val(new ValueUndefined(context));
    }
}

Val ValueString::operator[](const Val& v) {
    if (ValueString* va = dynamic_cast<ValueString*>(v.get())) {
        if (va->data == "charAt") {
            return Val(new BuiltInCharAt(context, data));
        } else if (va->data == "split") {
            return Val(new BuiltInSplit(context, data));
        } else {
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vs->getData()).get();
        return (*this)[val];
    }
    return Val(new ValueUndefined(context));
}

string ValueString::getRepr() {
    return data;
}


Val ValueBoolean::op(string, Val) {
    context->dump();
    return Val(new ValueUndefined(context));
}

string ValueBoolean::getRepr() {
    if (data)
        return "true";
    else
        return "false";
}
