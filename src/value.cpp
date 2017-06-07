#include "value.h"
#include "atom.h"
#include "interpr.h"
#include "builtins.h"

#include <boost/lexical_cast.hpp>


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
            assert(0);
        }
    } else if ((as=dynamic_cast<AtomSymbol*>(atom))) {
        string str = as->getRepr();
        data = Val(new ValueSymbol(context, str));
    } else {
        // TODO: implement
        assert(0);
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

Value Value::op(string op, Value v) {
    return Value(data->op(op, v.data));
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
    assert(0);
    return Val(new ValueUndefined(context));
}
Val ValueBase::member(Val) {
    assert(0);
    return Val(new ValueUndefined(context));
}
Val ValueBase::call(std::vector<Val>&) {
    assert(0);
    return Val(new ValueUndefined(context));
}
Val ValueBase::operator[](const Val&) {
    assert(0);
    return Val(new ValueUndefined(context));
}
string ValueBase::getRepr() {
    assert(0);
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
    } else {
        Value val = context->getValue(data);
        return val.get()->op(opr, v);
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
            assert(0);
            return Val(new ValueUndefined(context));
        }
        
    } else if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vs->getData()).get();
        return op(opr, val);
    } else {
        assert(0);
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
            assert(0);
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vs = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vs->getData()).get();
        return op(opr, val);
    } else {
        assert(0);
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
    assert(0);
    return Val(new ValueUndefined(context));
}

string ValueBoolean::getRepr() {
    if (data)
        return "true";
    else
        return "false";
}
