#pragma once
#include<array>
#include<string>
using std::array;
using std::string;

// [0]   "ident  "
// [1]   "strConst    "
// [2]   "numberConst "

const array<string, 48> KEYWORDS_STRINGS = {{
    "unused",         // unused
    "unused",         // unused
    "unused",         // unused
    ",",         // [3]
    ";",         // [4]
    "unused",         // unused (\n)
    "$",         // [6]
    "function",  // [7]
    "class",     // [8]
    "(",         // [9]
    ")",         // [10]
    "[",         // [11]
    "]",         // [12]
    "}",         // [13]
    "{",         // [14]
    "var",       // [15]
    "let",       // [16]
    "const",     // [17]
    "new",       // [18]
    "!",         // [19]
    "^",         // [20]
    "&",         // [21]
    "*",         // [22]
    "-",         // [23]
    "+",         // [24]
    "/",         // [25]
    "|",         // [26]
    "return",    // [27]
    "if",        // [28]
    "else",      // [29]
    "continue",  // [30]
    "break",     // [31]
    "do",        // [32]
    "while",     // [33]
    "for",       // [34]
    "=",         // [35]
    "+=",        // [36]
    "-=",        // [37]
    "*=",        // [38]
    "/=",        // [39]
    "^=",        // [40]
    "&=",        // [41]
    "|=",        // [42]
    "try",       // [43]
    "catch",     // [44]
    "finally",   // [45]
    "==",        // [46]
    "!=",        // [47]
}};

const array<char, 18> SPECIAL_CHARACTERS = {{
    ',',         // [3]
    ';',         // [4]
    '(',         // [9]
    ')',         // [10]
    '[',         // [11]
    ']',         // [12]
    '}',         // [13]
    '{',         // [14]
    '!',         // [19]
    '^',         // [20]
    '&',         // [21]
    '*',         // [22]
    '-',         // [23]
    '+',         // [24]
    '/',         // [25]
    '|',         // [26]
    '=',         // [35]
}};


class Atom {
public:
    virtual string getStr()const=0;
    virtual string getRepr()const=0;
};

class AtomKeyword : public Atom {
    unsigned key;
public:
    AtomKeyword(unsigned key);
    virtual string getStr()const;
    virtual string getRepr()const;
};

class AtomSymbol : public Atom {
    string str;
    unsigned id;
public:
    AtomSymbol(const string&, unsigned);
    virtual string getStr()const;
    virtual string getRepr()const;
};

class AtomConstant : public Atom {
    string str;
public:
    AtomConstant(const string&);
    virtual string getStr()const;
    virtual string getRepr()const;
};
