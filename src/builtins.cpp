#include "builtins.h"
#include "interpr.h"

#include <cassert>

Val BuiltInFunction::operator[](const Val& v) {
    if (ValueString* vs = dynamic_cast<ValueString*>(v.get())) {
        if (vs->getData() == "constructor") {
            return Val(new BuiltInFunction(context));
        } else {
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vsym = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vsym->getData()).get();
        return (*this)[val];
    }
    return Val(new ValueUndefined(context));
}

Val BuiltInFunction::call(std::vector<Val>& args) {
    std::cout<<"Function object constructor call detected.\n";
    std::cout<<"Arguments are: ( \n";
    for (auto a : args) {
        std::cout<<""<<a->getRepr()<<std::endl;
    }
    std::cout<<")"<<std::endl;
    // TODO: interpret it deeper?
    exit(0);
    return Val(new ValueUndefined(context));
}

string BuiltInFunction::getRepr() {
    string name = "Function";
    return  "function " + name + "() {\n    [native code]\n}";
}

Val BuiltInArray::operator[](const Val& v) {
    if (ValueString* vs = dynamic_cast<ValueString*>(v.get())) {
        if (vs->getData() == "constructor") {
            return Val(new BuiltInFunction(context));
        } else {
            return Val(new ValueUndefined(context));
        }
    } else if (ValueSymbol* vsym = dynamic_cast<ValueSymbol*>(v.get())) {
        Val val = context->getValue(vsym->getData()).get();
        return (*this)[val];
    }
    return Val(new ValueUndefined(context));
}

string BuiltInArray::getRepr() {
    string name = "Array";
    return  "function " + name + "() {\n    [native code]\n}";
}

Val BuiltInCharAt::call(std::vector<Val>& v) {
    if (v.empty()) {
        string str = "";
        str += data[0];
        return Val(new ValueString(context, str));
    } else if (ValueInteger* vi = dynamic_cast<ValueInteger*>(v[0].get())) {
        string str = "";
        str += data[vi->getData()];
        return Val(new ValueString(context, str));
    } else {
        string str = "";
        str += data[0];
        return Val(new ValueString(context, str));
    }
}

string BuiltInCharAt::getRepr() {
    string name = "charAt";
    return  "function " + name + "() {\n    [native code]\n}";
}
