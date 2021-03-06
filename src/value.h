#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
using std::string;
using std::vector;
using std::shared_ptr;

class Atom;
class AtomSymbol;
class Context;
class Node;

class ValueBase;
typedef shared_ptr<ValueBase> Val;

class Value {
    Context* context;
    Val data;
public:
    Value();
    Value(Context*, Atom*);
    Value(Context* c);
    Value(Val);
    Value(Context*, vector<Value>);
    Value(Context*, AtomSymbol*, vector<AtomSymbol*>, Node*); // function

    Value op(string op, Value);
    Value unaryOp(string op);
    Value member(Value); // operator "."
    Value call(std::vector<Value>&);
    Value operator[](const Value&);
    string getRepr();

    bool cond()const;

    Val get() { return data; }
};

class ValueBase {
protected:
    Context* context;
public:
    ValueBase(Context* c):context(c) {}

    virtual Val op(string op, Val);
    virtual Val unaryOp(string op);
    virtual Val member(Val); // operator "."
    virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();
    virtual bool cond()const { return true; }

    Context* getContext() { return context; }
};


class ValueSymbol : public ValueBase {
    string data;

public:
    ValueSymbol(Context* c, string str)
        :ValueBase(c), data(str) {}

    virtual Val op(string op, Val);
    virtual Val unaryOp(string op);
    // virtual Val member(Val);
    virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();

    string getData()const { return data; }
};

class ValueFunction : public ValueBase {
    string name;
    vector<string> params;
    Node* node;

public:
    ValueFunction(Context* c, string n, vector<string> p, Node* nod)
        :ValueBase(c), name(n), params(p), node(nod) {}

    // virtual Val op(string op, Val);
    // virtual Val member(Val);
    virtual Val call(std::vector<Val>&);
    // virtual Val operator[](const Val&);
    virtual string getRepr();
};

class ValueUndefined : public ValueBase {

public:
    ValueUndefined(Context* c)
        :ValueBase(c) {}

    virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    // virtual Val operator[](const Val&);
    virtual string getRepr() { return "undefined"; }
    virtual bool cond()const { return false; }
};

class ValueArray : public ValueBase {
    vector<Val> data;

public:
    ValueArray(Context* c, vector<Val> v)
        :ValueBase(c), data(v) {}

    // virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();
};

class ValueInteger : public ValueBase {
    int data;

public:
    ValueInteger(Context* c, int v)
        :ValueBase(c), data(v) {}

    virtual Val op(string op, Val);
    virtual Val unaryOp(string op);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();
    virtual bool cond()const { return data; }

    int getData()const {return data;}
};

class ValueFloat : public ValueBase {
    float data;

public:
    ValueFloat(Context* c, float v)
        :ValueBase(c), data(v) {}

    // virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    // virtual Val operator[](const Val&);
    virtual string getRepr() { return std::to_string(data); }
    virtual bool cond()const { return data; }

    float getData()const {return data;}
};

class ValueString : public ValueBase {
    string data;

public:
    ValueString(Context* c, string v);

    virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();
    virtual bool cond()const { return data.size(); }

    string getData()const {return data;}
};

class ValueBoolean : public ValueBase {
    bool data;

public:
    ValueBoolean(Context* c, bool v)
        :ValueBase(c), data(v) {}

    virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    // virtual Val operator[](const Val&);
    virtual string getRepr();
    virtual bool cond()const { return data; }

    bool getData()const {return data;}
};
