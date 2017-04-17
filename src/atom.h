#pragma once
#include<array>
#include<string>
using std::array;
using std::string;
// [0]   "ident  "
// [1]   "strConst    "
// [2]   "numberConst "

const array<string, 54> KEYWORDS_STRINGS = {{
    "++",        // [0]
    "--",        // [1]
    ".",         // [2]
    ",",         // [3]
    ";",         // [4]
    "unused",    // unused (\n)
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
    "&&",        // [48]
    "||",        // [49]
    "<",         // [50]
    "<=",        // [51]
    ">",         // [52]
    ">=",        // [53]
}};

const array<char, 20> SPECIAL_CHARACTERS = {{
    '.',         // [2]
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
    '<',         // [50]
    '>',         // [52]
}};


struct Atom {
    virtual string getStr()const=0;
    virtual string getRepr()const=0;
};

struct AtomKeyword : public Atom {
    unsigned key;

    AtomKeyword(unsigned key);
    virtual string getStr()const;
    virtual string getRepr()const;
};

struct AtomSymbol : public Atom {
    string str;
    unsigned id;

    AtomSymbol(const string&, unsigned);
    virtual string getStr()const;
    virtual string getRepr()const;
};

struct AtomConstant : public Atom {
    enum Type {
        String,
        Integer,
        Float,
    };
    string str;
    Type type;

    AtomConstant(const string&, Type);
    virtual string getStr()const;
    virtual string getRepr()const;
};
