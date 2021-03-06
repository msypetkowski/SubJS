#pragma once
#include "value.h"

class BuiltInPrint : public ValueBase {
public:
    BuiltInPrint(Context* c)
        :ValueBase(c) {}

    virtual Val call(std::vector<Val>& v) {
        for (unsigned a=0; a<v.size(); ++a) {
            if (a == v.size() - 1)
                std::cout << v[a]->getRepr();
            else
                std::cout << v[a]->getRepr() << " ";
        }
        std::cout<<std::endl;
        return Val(new ValueUndefined(context));
    }

    virtual string getRepr() {
        string name = "print";
        return  "function " + name + "() {\n    [native code]\n}";
    }
};

class BuiltInFunction : public ValueBase {
public:
    BuiltInFunction(Context* c)
        :ValueBase(c) {}

    virtual Val operator[](const Val&);
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInArray : public ValueBase {
public:
    BuiltInArray(Context* c)
        :ValueBase(c) {}

    virtual Val operator[](const Val&);
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInString : public ValueBase {
public:
    BuiltInString(Context* c)
        :ValueBase(c) {}

    virtual Val operator[](const Val&);
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInCharAt : public ValueBase {
    string data;
public:
    BuiltInCharAt(Context* c, string v)
        :ValueBase(c), data(v) {}
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInSplit : public ValueBase {
    string data;
public:
    BuiltInSplit(Context* c, string v)
        :ValueBase(c), data(v) {}
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInReverse : public ValueBase {
    vector<Val> data;
public:
    BuiltInReverse(Context* c, vector<Val> v)
        :ValueBase(c), data(v) {}
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInJoin : public ValueBase {
    vector<Val> data;
public:
    BuiltInJoin(Context* c, vector<Val> v)
        :ValueBase(c), data(v) {}
    virtual Val call(std::vector<Val>&);
    virtual string getRepr();
};

class BuiltInEval : public ValueBase {
public:
    BuiltInEval(Context* c)
        :ValueBase(c) {}

    virtual Val call(std::vector<Val>& args);

    virtual string getRepr() {
        string name = "eval";
        return  "function " + name + "() {\n    [native code]\n}";
    }
};
