#include "builtins.h"
#include "interpr.h"

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
