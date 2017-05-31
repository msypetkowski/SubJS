#include "value.h"
#include "atom.h"
#include "interpr.h"

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
    if (type == "undefined") {
        return Value(context);
    }
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
        if (type == "symbol")
            return context->getValue(stringData).call(args);
        if (type == "function" && stringData == "Function") {
            std::cout<<"Function object constructor call detected.\n";
            std::cout<<"Arguments are: ( \n";
            for (auto a : args) {
                std::cout<<""<<a.getRepr()<<std::endl;
            }
            std::cout<<")"<<std::endl;
            // TODO: interpret it deeper?
            return Value(context);
        }

        std::cout<<type<<std::endl;

        // TODO; implement
        assert(0);
    }
    assert(0);
    return Value(context);
}

Value Value::operator[](const Value& v) {
    if (type == "symbol")
        return context->getValue(stringData)[v];
    Value val = v;
    if (v.type == "symbol")
        val = context->getValue(v.stringData);

    if (val.type == "string" && val.stringData == "constructor") {
        Value ret = Value(context);
        ret.type = "function";
        if (type == "array") {
            ret.stringData = "Array";
        } else if (type == "function") {
            ret.stringData = "Function";
        } else {
            // TODO: implement
            std::cout<<type<<std::endl;
            assert(0);
        }
        return ret;
    }

    if (type == "array") {
        if (val.type == "int") {
            if (val.intData >= 0 && (unsigned)val.intData < arrayData.size()) {
                return arrayData[val.intData];
            } else return Value();
        } else return Value();
    } else {
        // undefined
        return Value();
    }
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
        return "undefined";
    else if (type == "array") {
        string ret;
        for(auto v:arrayData) {
            if (v.type == "undefined")
                ret += ",";
            else
                ret += v.getRepr() + ",";
        }
        ret = ret.substr(0, ret.size()-1);
        return ret;
    } else if (type == "function") {
        string ret = "function " + stringData + "() {\n    [native code]\n}";
        return ret;
    }
    assert(0);
    return "";
}
