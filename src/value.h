#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

class Atom;
class Context;

class Value {
    public: // TODO: remove
    string type;
    float floatData;
    int intData;
    string stringData;
    std::vector<Value> arrayData;
    Context* context;
public:
    Value():type("undefined"){};
    Value(Context*, Atom*);
    Value(Context* c):type("undefined"),context(c){};

    Value op(string op, Value);
    Value member(Value); // operator "."
    Value call(std::vector<Value>&);
    Value operator[](const Value&);
    string getRepr();
};
