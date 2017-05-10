#include "atom.h"

#include <ostream>

static std::map<string, int> createKeywordsMap() {
    std::map<string, int> ret;
    for (unsigned i=0; i<KEYWORDS_STRINGS.size(); ++i) {
        ret[KEYWORDS_STRINGS[i]] = i;
    }
    return ret;
}
std::map<string, int> KEYWORDS_MAP = createKeywordsMap();

AtomKeyword::AtomKeyword(AtomPos p, int k):Atom(p) {
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

int AtomKeyword::getGrammarSymbolID()const {
    return key;
}

AtomSymbol::AtomSymbol(AtomPos p, const string& s, int i)
        :Atom(p), str(s), id(i) {
}

string AtomSymbol::getStr()const {
    return "Symbol " + std::to_string(id) + " : " + str;
}

string AtomSymbol::getRepr()const {
    return str;
}

int AtomSymbol::getGrammarSymbolID()const {
    return -1;
}

AtomConstant::AtomConstant(AtomPos p, const string& s, Type t):Atom(p) {
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

int AtomConstant::getGrammarSymbolID()const {
    return -2;
}

static int getKeywordId(const string& s) {
    if (!KEYWORDS_MAP.count(s)) {
        throw std::string("Wrong keyword string");
    }
    return KEYWORDS_MAP[s];
}

SymSet::SymSet(std::initializer_list<const string> l) {
    for (string s : l) {
        if (s=="SYMBOL")
            includeSymbol();
        else if (s=="CONSTANT")
            includeConstant();
        else data.insert(getKeywordId(s));
    }
}

// SymSet::SymSet(std::vector<const string> l) {
//     for (string s : l) {
//         if (s=="SYMBOL")
//             includeSymbol();
//         else if (s=="CONSTANT")
//             includeConstant();
//         else data.insert(KEYWORDS_MAP[s]);
//     }
// }

bool SymSet::hasKeyword(const string&s )const {
    return data.count(getKeywordId(s));
}
bool SymSet::hasSymbol()const {
    return data.count(-1);
}
bool SymSet::hasConstant()const {
    return data.count(-2);
}

bool SymSet::has(const Atom* a)const {
    return data.count(a->getGrammarSymbolID());
}

SymSet SymSet::operator+(const SymSet &t)const {
    SymSet ss(*this);
    ss.data.insert(t.data.begin(), t.data.end());
    return ss;
}

std::ostream& operator<< (std::ostream& os, const SymSet& t) {
    os << '{';
    for (auto a: t.data) {
        if (a >= 0) {
            os << KEYWORDS_STRINGS[a] << ",";
        } else if (a == -1) {
            os << "SYMBOL ,";
        } else if (a == -2) {
            os << "CONSTANT ,";
        }
    }
    return os << '}';
}
