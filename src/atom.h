#pragma once
#include<array>
#include<string>
#include<set>
#include<map>
using std::array;
using std::string;

const array<string, 56> KEYWORDS_STRINGS = {{
    "++",        // [0]
    "--",        // [1]
    ".",         // [2]
    ",",         // [3]
    ";",         // [4]
    "___unused", // unused (\n)
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
    "%",         // [54]
    "%=",        // [55]
}};

// extern const std::map<string, int> KEYWORDS_MAP;

const array<char, 21> SPECIAL_CHARACTERS = {{
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
    '%',         // [54]
}};

typedef std::pair<unsigned, unsigned> AtomPos;

struct Atom {
    AtomPos pos;
    virtual string getStr()const=0;
    virtual string getRepr()const=0;
    virtual int getGrammarSymbolID()const=0;

    Atom(const AtomPos& p): pos(p) { }
    AtomPos getPos()const { return pos; }
};

struct AtomKeyword : public Atom {
    int key;

    AtomKeyword(AtomPos p, int key);
    virtual string getStr()const;
    virtual string getRepr()const;
    virtual int getGrammarSymbolID()const;
};

struct AtomSymbol : public Atom {
    string str;
    int id;

    AtomSymbol(AtomPos p, const string&, int);
    virtual string getStr()const;
    virtual string getRepr()const;
    virtual int getGrammarSymbolID()const;
};

struct AtomConstant : public Atom {
    enum Type {
        String,
        Integer,
        Float,
    };
    string str;
    Type type;

    AtomConstant(AtomPos p, const string&, Type);
    virtual string getStr()const;
    virtual string getRepr()const;
    virtual int getGrammarSymbolID()const;
};

class SymSet {
    std::set<int> data;
public:
    SymSet(std::initializer_list<const string>);
    void includeSymbol() { data.insert(-1); }
    void includeConstant() { data.insert(-1); }

    bool hasKeyword(const string&)const;
    bool hasSymbol()const;
    bool hasConstant()const;
    bool has(const Atom*)const;
    SymSet operator+(const SymSet &t)const;
    unsigned size() { return data.size(); }

    friend std::ostream& operator<< (std::ostream& os, const SymSet& t);
};
