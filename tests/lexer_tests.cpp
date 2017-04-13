#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include <iostream>

#define private public
#define protected public

#include "../src/lexer.h"
#include "../src/atom.h"

namespace LexerTests
{
    BOOST_AUTO_TEST_CASE(testKeywords1) {
        string code = "(){}[]=";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 8);
        code += "$";
        for(unsigned i=0; i<atoms.size(); ++i) {
            BOOST_CHECK_EQUAL(string("") + code[i], atoms[i]->getRepr());
        }
        BOOST_CHECK_EQUAL("$" , (*atoms.rbegin())->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testKeywords2) {
        string code = "";
        for (auto k : KEYWORDS_STRINGS) {
            if (k == "$") {
                code += "endOfFile ";
            } else code += (k + " ");
        }
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), KEYWORDS_STRINGS.size() + 1);
        for(unsigned i=0; i<KEYWORDS_STRINGS.size(); ++i) {
            if (KEYWORDS_STRINGS[i] != "$")
                BOOST_CHECK_EQUAL(KEYWORDS_STRINGS[i], atoms[i]->getRepr());
        }
        BOOST_CHECK_EQUAL("$" , (*atoms.rbegin())->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testIgnoreWhitespace) {
        string code = "(\n)\n \t {}\n[  \t]";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 7);
    }

    BOOST_AUTO_TEST_CASE(testGeneral1) {
        string code = "for(var a=1;;)";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 10);
        BOOST_CHECK_EQUAL("$" , (*atoms.rbegin())->getRepr());
    }

}
