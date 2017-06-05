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
};

class BuiltInFunction : public ValueBase {
public:
    BuiltInFunction(Context* c)
        :ValueBase(c) {}

    virtual Val operator[](const Val&);
    virtual string getRepr();
};

class BuiltInArray : public ValueBase {
public:
    BuiltInArray(Context* c)
        :ValueBase(c) {}

    virtual Val operator[](const Val&);
    virtual string getRepr();
};