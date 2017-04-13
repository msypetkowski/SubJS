#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include <iostream>

// #define private public
// #define protected public

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

            BOOST_CHECK(dynamic_cast<AtomKeyword*>(atoms[i]));
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
            if (KEYWORDS_STRINGS[i] != "$") {
                BOOST_CHECK_EQUAL(KEYWORDS_STRINGS[i], atoms[i]->getRepr());

                BOOST_CHECK(dynamic_cast<AtomKeyword*>(atoms[i]));
                BOOST_CHECK(dynamic_cast<AtomKeyword*>(atoms[i])->key == i);
                BOOST_CHECK(!dynamic_cast<AtomConstant*>(atoms[i]));
            }
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

    BOOST_AUTO_TEST_CASE(testStringConstant) {
        string code = "var a = 'text'";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 5);
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3])->type ==
                AtomConstant::Type::String);
        BOOST_CHECK_EQUAL(atoms[3]->getRepr(), "text");
    }

    BOOST_AUTO_TEST_CASE(testNumberConstant) {
        string code = "var a = 123 ; var b = 1.003";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 10);
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3])->type ==
                AtomConstant::Type::Integer);
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[8]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[8])->type ==
                AtomConstant::Type::Float);
        BOOST_CHECK_EQUAL(atoms[3]->getRepr(), "123");
        BOOST_CHECK_EQUAL(atoms[8]->getRepr(), "1.003");
    }

    BOOST_AUTO_TEST_CASE(testFloatConstant) {
        string code = "1.02 .5 12.";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 4);

        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[0]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[0])->type ==
                AtomConstant::Type::Float);
        BOOST_CHECK_EQUAL(atoms[0]->getRepr(), "1.02");

        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[1]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[1])->type ==
                AtomConstant::Type::Float);
        BOOST_CHECK_EQUAL(atoms[1]->getRepr(), ".5");

        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[2]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[2])->type ==
                AtomConstant::Type::Integer);
        BOOST_CHECK_EQUAL(atoms[2]->getRepr(), "12");
    }

    BOOST_AUTO_TEST_CASE(testGeneral1) {
        string code = "for(var a=1;;)++";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 11);
        BOOST_CHECK_EQUAL("$" , (*atoms.rbegin())->getRepr());
    }

}
