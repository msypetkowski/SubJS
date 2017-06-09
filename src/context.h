#pragma once

#include "atom.h"
#include "tree.h"
#include "value.h"

#include <map>

class Interpreter;

class Context {
    Interpreter& interpr;
    std::map<string, Value> variables;
    vector<string> strings;
public:
    Context(Interpreter&);
    void addVariable(string varName, Value value);
    Value getValue(string);

    void logString(string);

    Value callNode(Node*);

    void dump(bool isFinal=false);
    void printStrings();
};

