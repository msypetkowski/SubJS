#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
using std::string;
using std::vector;
using std::shared_ptr;

class Atom;
class Context;

// class Value {
//     public: // TODO: remove
//     string type;
//     float floatData;
//     int intData;
//     string stringData;
//     std::vector<Value> arrayData;
//     Context* context;
// public:
//     Value():type("undefined"){};
//     Value(Context*, Atom*);
//     Value(Context* c):type("undefined"),context(c){};
// 
//     Value op(string op, Value);
//     Value member(Value); // operator "."
//     Value call(std::vector<Value>&);
//     Value operator[](const Value&);
//     string getRepr();
// };
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

    Value op(string op, Value);
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
    // virtual Val member(Val);
    virtual Val call(std::vector<Val>&);
    virtual Val operator[](const Val&);
    virtual string getRepr();

    string getData()const { return data; }
};

class ValueFunction : public ValueBase {
    string name;
    //TODO: node data

public:
    ValueFunction(Context* c, string nam)
        :ValueBase(c), name(nam) {}

    // virtual Val op(string op, Val);
    // virtual Val member(Val);
    // virtual Val call(std::vector<Val>&);
    // virtual Val operator[](const Val&);
    // virtual string getRepr();
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
    // virtual string getRepr();
    virtual bool cond()const { return data; }

    float getData()const {return data;}
};

class ValueString : public ValueBase {
    string data;

public:
    ValueString(Context* c, string v)
        :ValueBase(c), data(v) {}

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
