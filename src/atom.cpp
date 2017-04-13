#include "atom.h"

AtomKeyword::AtomKeyword(unsigned k) {
    key = k;
}

string AtomKeyword::getStr()const {
    string ret = "Keyword " + std::to_string(key) + " : ";
    string str = KEYWORDS_STRINGS[key];
    if (str == "\n")
        ret += "\\n";
    else ret += str;
    return ret;
}
string AtomKeyword::getRepr()const {
    return KEYWORDS_STRINGS[key];
}

AtomSymbol::AtomSymbol(const string& s, unsigned i):
        str(s), id(i) {
}

string AtomSymbol::getStr()const {
    return "Symbol " + std::to_string(id) + " : " + str;
}

string AtomSymbol::getRepr()const {
    return str;
}

AtomConstant::AtomConstant(const string& s, Type t) {
    str = s;
    type = t;
}

string AtomConstant::getStr()const {
    if (type == Type::String)
        return "Constant String: " + str;
    if (type == Type::Integer)
        return "Constant Integer: " + str;
    return "Constant Float: " + str;
}

string AtomConstant::getRepr()const {
    return str;
}
