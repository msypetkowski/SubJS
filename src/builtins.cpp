#include "builtins.h"
#include "interpr.h"

#include <cassert>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>

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

Val BuiltInArray::call(std::vector<Val>& args) {
    if (args.empty()) {
        return Val(new ValueArray(context, {Val(new ValueUndefined(context))}));
    } else {
        if (ValueInteger* vi = dynamic_cast<ValueInteger*>(args[0].get())) {
            vector<Val> ret;
            for (int i=0; i<vi->getData(); ++i) {
                ret.push_back(Val(new ValueUndefined(context)));
            }
            return Val(new ValueArray(context, ret));
        } else {
            return Val(new ValueArray(context, {Val(new ValueUndefined(context))}));
        }
    }
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

Val BuiltInSplit::call(std::vector<Val>& v) {
    if (v.empty()) {
        Val strVal = Val(new ValueString(context, data));
        return Val(new ValueArray(context, {strVal}));
    } else if (ValueString* vs = dynamic_cast<ValueString*>(v[0].get())) {

        vector<Val> vec;

        vector<string> splitVec;
        string d = vs->getData();
        if (d != "") {
            boost::split( splitVec, data, boost::is_any_of(d), boost::token_compress_on );
            for(string s : splitVec) {
                vec.push_back(Val(new ValueString(context, s)));
            }
            return Val(new ValueArray(context, vec));
        } else {
            for(char c : data) {
                string str;
                str+=c;
                vec.push_back(Val(new ValueString(context, str)));
            }
            return Val(new ValueArray(context, vec));
        }
    } else {
        assert(0);
        return Val(new ValueUndefined(context));
    }
}
string BuiltInSplit::getRepr() {
    string name = "split";
    return  "function " + name + "() {\n    [native code]\n}";
}

Val BuiltInReverse::call(std::vector<Val>&) {
    std::vector<Val> vec=data;
    std::reverse(vec.begin(), vec.end());
    return Val(new ValueArray(context, vec));
}
string BuiltInReverse::getRepr() {
    string name = "reverse";
    return  "function " + name + "() {\n    [native code]\n}";
}


Val BuiltInJoin::call(std::vector<Val>& v) {
    if (v.empty()) {
        vector<string> strings;
        for (auto e : data)
            strings.push_back(e->getRepr());
        string ret = boost::algorithm::join(strings, ",");
        return Val(new ValueString(context, ret));
    } else if (ValueString* vs = dynamic_cast<ValueString*>(v[0].get())) {
        vector<string> strings;
        for (auto e : data)
            strings.push_back(e->getRepr());
        string ret = boost::algorithm::join(strings, vs->getData());
        return Val(new ValueString(context, ret));
    } else {
        assert(0);
        return Val(new ValueUndefined(context));
    }
}
string BuiltInJoin::getRepr() {
    string name = "join";
    return  "function " + name + "() {\n    [native code]\n}";
}
