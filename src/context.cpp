#include "context.h"
#include "interpr.h"
#include "builtins.h"
#include "config.h"

#include <iostream>
#include <cassert>
using namespace std;


static vector<pair<string, Value>> eliminateSubstrings(map<string, Value> elements) {
    vector<pair<string,Value>> newData;
    // TODO: optimize (now it's brute force)
    for (auto e : elements) {
        bool found = false;
        string cur = e.second.getRepr();
        for (auto f : elements) {
            if (f.first == e.first
                    || f.second.getRepr() == cur)
                continue;
            if (f.second.getRepr().find(cur) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found)
            newData.push_back(e);
    }
    return newData;
}

static vector<string> eliminateSubstrings(vector<string> strings) {
    vector<string> ret;
    for (string i : strings ) {
        bool found = false;
        for (string j : strings)  {
            if (i == j)
                continue;
            if (j.find(i) != std::string::npos) {
                found = true;
                break;
            }
        }
        if (!found)
            ret.push_back(i);
    }
    return ret;
}

Context::Context(Interpreter& i)
    :interpr(i) {
}

void Context::addVariable(string varName, Value value) {
    variables[varName] = value;
}

Value Context::getValue(string name) {
    if (!variables.count(name)) {
        // assert(variables.count("undefined"));
        return Value(this);
    }
    return variables[name];
};

void Context::logString(string s) {
    if (CONFIG::STRINGS) {
        strings.push_back(s);
    }
}

Value Context::callNode(Node* n) {
    assert(n->name == "CompoundStatement");
    return interpr.CompoundStatement(n);
}

void Context::dump(bool isFinal) {
    if (isFinal || (CONFIG::DUMP && !CONFIG::IGNORE)) {
        cout << "Context dump: " << endl;
        cout << "Variables count: " << variables.size() << endl;
        for (pair<string,Value> p : eliminateSubstrings(variables)) {
            if (p.second.getRepr().size() < 6)
                continue;
            if (dynamic_cast<ValueFunction*>(p.second.get().get()))
                continue;
            if (dynamic_cast<BuiltInPrint*>(p.second.get().get()))
                continue;
            if (dynamic_cast<BuiltInEval*>(p.second.get().get()))
                continue;
            cout << "-----------------------------" << endl;
            cout << p.first << ": " << p.second.getRepr() << endl;
        }
    }
    if (!CONFIG::IGNORE)
        assert(0);
}

void Context::printStrings() {
    cout << "/-------------\\" << endl;
    cout << "|Strings:    |" << endl;
    cout << "\\-------------/" << endl;
    for (auto s : eliminateSubstrings(strings)) {
    //for (auto s : strings) {
        cout << s << endl;
    }
}
