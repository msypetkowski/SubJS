#include "lexer.h"

#include <algorithm>
#include <iostream> // TODO: rm line

#include <boost/algorithm/string/predicate.hpp>

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || c == '_';
}

bool isHexLetter(char c) {
    return (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

bool isNumber(char c) {
    return (c >= '0' && c <= '9');
}

bool isEndl(char c) {
    return c == '\n';
}

bool Lexer::isWhitespace(char c) {
    return c == ' '
        || c == '\t'
        || c == '\r'
        || (!treatEndlAsSeparator && c == '\n');
}

bool Lexer::isSpecial(char c) {
    return std::find(SPECIAL_CHARACTERS.begin(), SPECIAL_CHARACTERS.end(), c) != SPECIAL_CHARACTERS.end()
        || (treatEndlAsSeparator && c == '\n');
}

bool Lexer::getKeyword() {
    while (isWhitespace(code[pos]))
        ++pos;
    unsigned wordEnd = pos;
    if (isLetter(code[pos])) {
        string word;
        while(isLetter(code[wordEnd])) {
            word += code[wordEnd];
            ++wordEnd;
        }
        auto keyIter = std::find(KEYWORDS_STRINGS.begin(), KEYWORDS_STRINGS.end(), word);
        if (keyIter != KEYWORDS_STRINGS.end()) {
            newKeywordAtom(std::distance(KEYWORDS_STRINGS.begin(), keyIter));
            pos = wordEnd;
            return true;
        } else return false;
    }

    string word;
    int bestMatch=-1; // longest match
    while (1) {
        word += code[wordEnd];
        ++wordEnd;
        bool anyMatch = false;
        for (unsigned k=0; k < KEYWORDS_STRINGS.size(); ++k) {
            string keystr = KEYWORDS_STRINGS[k];
            if (!isSpecial(keystr[0]))
                continue;
            if (keystr == word) {
                bestMatch = k;
            }
            if (boost::starts_with(keystr, word)) {
                anyMatch = true;
            }
        }
        if (!anyMatch)
            break;
    }
    if (bestMatch != -1) {
        newKeywordAtom(bestMatch);
        pos += KEYWORDS_STRINGS[bestMatch].size();
        return true;
    }
    return false;
}

bool Lexer::getSymbol() {
    while (isWhitespace(code[pos]))
        ++pos;
    auto newPos = pos;
    string word;
    if (isLetter(code[newPos])){
        while(isLetter(code[newPos]) || isNumber(code[newPos])){
            word += code[newPos];
            ++newPos;
        }
    }
    if (word != "") {
        if (symbolsMap.find(word) == symbolsMap.end()) {
            symbolsMap[word] = nextNewSymbolId++;
        }
        newSymbolAtom(word);
        pos = newPos;
        return true;
    }
    return false;
}

bool Lexer::getConstant() {
    while (isWhitespace(code[pos]))
        ++pos;
    char first = code[pos];
    if (first == '"' || first == '\'') {
        unsigned newPos = pos + 1;
        string word = "";
        while (code[newPos] != '$') {
            if (code[newPos] == first) {
                unsigned count = 1;
                while(code[newPos - count] == '\\')
                    ++count;
                if (count&1)
                    break;
            }
            if (code[newPos] == '\n')
                return false;
            word += code[newPos];
            ++newPos;
        }
        if (code[newPos] == '$') {
            return false;
        } else {
            newConstantStringAtom(word);
            pos = newPos + 1;
            return true;
        }
    }

    if (first == '.') {
        unsigned newPos = pos + 1;
        string word = ".";
        while (code[newPos] != '$' && isNumber(code[newPos])) {
            word += code[newPos];
            ++newPos;
        }
        if (word == ".")
            return false;
        newConstantFloatAtom(word);
        pos = newPos;
        return true;
    }

    if (isNumber(first)) {
        unsigned newPos = pos;
        string word = "";
        while (code[newPos] != '$' && isNumber(code[newPos])) {
            word += code[newPos];
            ++newPos;
        }
        if (code[newPos] == '.') {
            word += code[newPos];
            ++newPos;
            while (code[newPos] != '$' && isNumber(code[newPos])) {
                word += code[newPos];
                ++newPos;
            }
            if (*word.rbegin() == '.') {
                if (!isLetter(code[newPos])) {
                    newConstantIntegerAtom(word.substr(0, word.size()-1));
                } else {
                    return false;
                }
            } else {
                newConstantFloatAtom(word);
            }
        } else if(code[newPos] == 'x' && word == "0") {
            word += code[newPos];
            ++newPos;
            while (code[newPos] != '$' &&
                    (isNumber(code[newPos]) || isHexLetter(code[newPos]))) {
                word += code[newPos];
                ++newPos;
            }
            if (*word.rbegin() == 'x') {
                return false;
            } else {
                if (!isLetter(code[newPos])) {
                newConstantIntegerAtom(word);
                } else return false;
            }
        } else if (!isLetter(code[newPos])) {
            newConstantIntegerAtom(word);
        } else return false;
        pos = newPos;
        return true;
    }

    return false;
}

bool Lexer::getComment(){
    while (isWhitespace(code[pos]))
        ++pos;
    if (code[pos] == '/' && code[pos+1] == '/') {
        while (code[pos] != '\n' && code[pos] != '$')
            ++pos;
        return true;
    }

    if (code[pos] == '/' && code[pos+1] == '*') {
        unsigned newPos = pos + 2;
        while ((code[newPos] != '*' ||  code[newPos+1] != '/')
                    && code[newPos+1] != '$'
                ) {
            ++newPos;
        }
        if (code[newPos + 1] == '$') {
            errors.push_back({line[pos], pos - lastEndl[pos] - 1});
            pos = newPos + 1;
            return false;
        } else {
            pos = newPos + 2;
            return true;
        }
    }
    return false;
}

Lexer::Lexer(Source& code1):
        code(code1),
        nextNewSymbolId(0),
        pos(0),
        treatEndlAsSeparator(false) {
    code += '$';

    // for finding error rows and collumns
    lastEndl.resize(code.size());
    line.resize(code.size());
    lastEndl[0] = -1;
    line[0] = 0;
    for(unsigned i = 1; i < code.size(); ++i) {
        if (isEndl(code[i])) {
            lastEndl[i] = i;
            line[i] = line[i-1] + 1;
        } else {
            lastEndl[i] = lastEndl[i - 1];
            line[i] = line[i-1];
        }
    }
}

bool Lexer::run(){
    Atom* newAtom = getNextAtom();
    while (newAtom == nullptr || newAtom->getRepr() != "$")
        newAtom = getNextAtom();
    if (errors.empty())
        return true;
    else return false;
}

vector<Atom*> Lexer::getAtoms() {
    return atoms;
}

vector<std::pair<unsigned,unsigned>> Lexer::getErrors() {
    return errors;
}

Atom* Lexer::getNextAtom() {
    if (!atoms.empty() && (*atoms.rbegin())->getRepr() == "$") {
        std::cout << ("Trying to get atom after end of file.") << std::endl;
        throw string("Trying to get atom after end of file.");
    }
    if (pos >= code.size()) {
        newKeywordAtom(6);
        return *atoms.rbegin();
    }

    while(getComment());
    if (!getConstant() && !getKeyword() && !getSymbol()) {
        if (code[pos] == '$'){
            newKeywordAtom(6);
            return *atoms.rbegin();
        }

        errors.push_back({line[pos], pos - lastEndl[pos] - 1});
        while(!isWhitespace(code[pos]))
            ++pos;
        return nullptr;
    } else {
        return *atoms.rbegin();
    }
}

std::pair<unsigned,unsigned> Lexer::getLastError() {
    if (errors.empty())
        throw string("Trying to get error while there is no errors.");
    return *errors.rbegin();
}
