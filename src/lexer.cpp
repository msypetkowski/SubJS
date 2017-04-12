#include "lexer.h"

#include <algorithm>
#include <iostream> // TODO: rm line

#include <boost/algorithm/string/predicate.hpp>

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z');
}

bool Lexer::isWhitespace(char c) {
    return c == ' '
        || c == '\t'
        || (treatEndlAsSeparator && c == '\n');
}

bool Lexer::isSpecial(char c) {
    return std::find(SPECIAL_CHARACTERS.begin(), SPECIAL_CHARACTERS.end(), c) != SPECIAL_CHARACTERS.end()
        || (!treatEndlAsSeparator && c == '\n');
}

bool Lexer::getKeyword() {
    while (isWhitespace(code[pos]))
        ++pos;
    unsigned wordEnd = pos;
    if (isLetter(code[pos])) {
        string word;
        while(isLetter(code[wordEnd])){
            word += code[wordEnd];
            ++wordEnd;
        }
        auto keyIter = std::find(KEYWORDS_STRINGS.begin(), KEYWORDS_STRINGS.end(), word);
        if (keyIter != KEYWORDS_STRINGS.end()) {
            atoms.push_back(new AtomKeyword(std::distance(KEYWORDS_STRINGS.begin(), keyIter)));
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
        atoms.push_back(new AtomKeyword(bestMatch));
        pos += KEYWORDS_STRINGS[bestMatch].size();
        return true;
    }
    return false;
}

bool Lexer::getSymbol() {
    while (isWhitespace(code[pos]))
        ++pos;
    string word;
    while(isLetter(code[pos])){
        word += code[pos];
        ++pos;
    }
    if (word != "") {
        if (symbolsMap.find(word) == symbolsMap.end()) {
            symbolsMap[word] = nextNewSymbolId++;
        }
        atoms.push_back(new AtomSymbol(word, symbolsMap[word]));
        return true;
    }
    return false;
}

bool Lexer::getConstant() {
    // TODO
    return false;
}

Lexer::Lexer(const string& code1):
        code(code1),
        nextNewSymbolId(0),
        pos(0),
        treatEndlAsSeparator(false) {
}

bool Lexer::run(){
    while (pos < code.size()) {
        if (!getKeyword() && !getSymbol() && !getConstant()) {
            errors.push_back(pos);
            // TODO: better error handling
            return false;
        }
    }
    return true;
}

vector<Atom*> Lexer::getAtoms() {
    return atoms;
}

vector<unsigned> Lexer::getErrors() {
    return errors;
}
