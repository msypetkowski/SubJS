#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include <boost/range/combine.hpp>

#include <iostream>
#include <array>

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

    BOOST_AUTO_TEST_CASE(testStringConstant1) {
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

    BOOST_AUTO_TEST_CASE(testStringConstant2) {
        string code = "var a = 'text \\n \"text\"'";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 5);
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3]));
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[3])->type ==
                AtomConstant::Type::String);
        BOOST_CHECK_EQUAL(atoms[3]->getRepr(), "text \\n \"text\"");
    }

    BOOST_AUTO_TEST_CASE(testStringConstant3) {
        string code = R"foo(
var a='"some" \n "reversed" var a="string"'["split"]('')["reverse"]()["join"]('');
        )foo";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 22 + 1);
        BOOST_CHECK_EQUAL(atoms[3]->getRepr(), R"foo("some" \n "reversed" var a="string")foo");
        BOOST_CHECK_EQUAL(atoms[5]->getRepr(), "split");
        BOOST_CHECK_EQUAL(atoms[8]->getRepr(), "");
    }

    BOOST_AUTO_TEST_CASE(testStringConstant4) {
        string code = R"foo(
a='foo\'' b="\"foo\\" c='foo\\\''
        )foo";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 9 + 1);
        BOOST_CHECK_EQUAL(atoms[2]->getRepr(), R"(foo\')");
        BOOST_CHECK_EQUAL(atoms[5]->getRepr(), R"(\"foo\\)");
        BOOST_CHECK_EQUAL(atoms[8]->getRepr(), R"(foo\\\')");
    }

    BOOST_AUTO_TEST_CASE(testStringConstant5) {
        string code = R"foo(
a='foo\'' b="\"foo\\" c='notClosed\'
        )foo";
        Lexer l(code);

        BOOST_CHECK(!l.run());
        BOOST_CHECK_EQUAL(l.getErrors().size(), 1);
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

    BOOST_AUTO_TEST_CASE(testNumberConstant2) {
        string code1 = "0x123 1x1";
        Lexer l1(code1);
        BOOST_CHECK(!l1.run());

        string code2 = "0x123 . 1";
        Lexer l2(code2);
        BOOST_CHECK(l2.run());
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
    BOOST_AUTO_TEST_CASE(testSymbolAtom) {
        string code = "_1 abc fda1__4";
        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 4);
        atoms.pop_back();
        for(auto a : atoms) {
            BOOST_CHECK(dynamic_cast<AtomSymbol*>(a));
        }
    }

    BOOST_AUTO_TEST_CASE(testGeneral1) {
        string code = "for(var a=1;;)++";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 11);
        BOOST_CHECK_EQUAL("$" , (*atoms.rbegin())->getRepr());
        BOOST_CHECK_EQUAL("=" , (atoms[4])->getRepr());
        BOOST_CHECK_EQUAL("1" , (atoms[5])->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testGeneral2) {
        string code = " var uradtodeg = \"\\x74h\";\n var um = \"l\\x65ng\";\n var ucollisionend = \"n\\x67th\";\n";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 15 + 1);
        BOOST_CHECK_EQUAL("var" , (atoms[0])->getRepr());
        BOOST_CHECK_EQUAL("uradtodeg" , (atoms[1])->getRepr());
        BOOST_CHECK_EQUAL("=", (atoms[2])->getRepr());
        BOOST_CHECK_EQUAL("\\x74h", (atoms[3])->getRepr());

        BOOST_CHECK_EQUAL("n\\x67th", (atoms[13])->getRepr());
        BOOST_CHECK_EQUAL(";", (atoms[14])->getRepr());
        BOOST_CHECK_EQUAL("$", (atoms[15])->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testGeneral3) {
        string code = "acollisionend[(function mmi(){return mothumbnailslist;}()) + mhtc + mrenderer](bmsg, 8740 - 8738);";
        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 28 + 1);

        BOOST_CHECK_EQUAL("mothumbnailslist", (atoms[9])->getRepr());
        BOOST_CHECK_EQUAL("8738", (atoms[25])->getRepr());
        BOOST_CHECK_EQUAL(")", (atoms[26])->getRepr());
        BOOST_CHECK_EQUAL(";", (atoms[27])->getRepr());
        BOOST_CHECK_EQUAL("$", (atoms[28])->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testGeneral4) {
        string code = R"foo(
function radtodeg(arenderer)
{
	if (arenderer[0] == 0x4D && arenderer[1] == 0x5a)
		{return true;}
)foo";

        Lexer l(code);

        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        BOOST_CHECK_EQUAL(atoms.size(), 27 + 1);
        BOOST_CHECK_EQUAL("function", atoms[0]->getRepr());
        BOOST_CHECK_EQUAL("{", atoms[5]->getRepr());
        BOOST_CHECK_EQUAL("(", atoms[7]->getRepr());
        BOOST_CHECK_EQUAL("0x4D", atoms[13]->getRepr());
        BOOST_CHECK_EQUAL("&&", atoms[14]->getRepr());
        BOOST_CHECK_EQUAL("0x5a", atoms[20]->getRepr());
        BOOST_CHECK(dynamic_cast<AtomConstant*>(atoms[20]));
        BOOST_CHECK_EQUAL(")", atoms[21]->getRepr());
        BOOST_CHECK_EQUAL("{", atoms[22]->getRepr());
        BOOST_CHECK_EQUAL("true", atoms[24]->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testGeneral5) {
        string code = R"foo(
var ami = new Array();
for (var mi = 0; mi < htc[jmsg(idoxie) + jcollisionend(imsg) + iparticleradius]; mi++)
{
    var renderer = htc.charCodeAt(mi);
    if (renderer < 128)
        {var atends = renderer;}
    else
        {var atends = am[renderer];}
    ami.push(atends);
};
)foo";
        std::array<string, 80> expectedAtoms = {{
            "var", "ami", "=", "new", "Array", "(", ")",
            ";", "for", "(", "var", "mi", "=", "0",
            ";", "mi", "<", "htc", "[", "jmsg", "(", "idoxie",
            ")", "+", "jcollisionend", "(", "imsg", ")",
            "+", "iparticleradius", "]", ";", "mi", "++",
            ")", "{", "var", "renderer", "=", "htc", ".",
            "charCodeAt", "(", "mi", ")", ";", "if", "(",
            "renderer", "<", "128", ")", "{", "var", "atends", "=",
            "renderer", ";", "}", "else", "{", "var", "atends", "=",
            "am", "[", "renderer", "]", ";", "}", "ami",
            ".", "push", "(", "atends", ")", ";", "}", ";", "$",
        }};

        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 80);

        for (auto tup : boost::combine(atoms, expectedAtoms)) {
            string s;
            Atom* a;
            boost::tie(a,s) = tup;
            BOOST_CHECK_EQUAL(a->getRepr(), s);
        }
    }

    BOOST_AUTO_TEST_CASE(testFail1) {
        string code = R"foo(
var ami = new Array();
for (var mi = 1x; mi < htc[jmsg(idoxie) + jcollisionend(2.imsg) + iparticleradius]; mi++)
{
    var 1renderer = htc.charCodeAt(mi);
    if (renderer < 0x12h)
        {var atends = renderer;}
    else
        {var atends = am[renderer];}
    ami.push(atends);
};
)foo";
        Lexer l(code);
        BOOST_CHECK(!l.run());
        auto errors = l.getErrors();
        BOOST_CHECK_EQUAL(errors.size(), 4);
        BOOST_CHECK_EQUAL(errors[0].first, 1);
        BOOST_CHECK_EQUAL(errors[0].second, 14);
        BOOST_CHECK_EQUAL(errors[1].first, 1);
        BOOST_CHECK_EQUAL(errors[1].second, 56);
        BOOST_CHECK_EQUAL(errors[2].first, 3);
        BOOST_CHECK_EQUAL(errors[2].second, 8);
        BOOST_CHECK_EQUAL(errors[3].first, 4);
        BOOST_CHECK_EQUAL(errors[3].second, 19);
    }

    BOOST_AUTO_TEST_CASE(testComments1) {
        string code = "a = 1 // this \r \t  is comment ***";
        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 4);
    }

    BOOST_AUTO_TEST_CASE(testComments2) {
        string code = "a = 1 /* /* this is comment ***/  a*4";
        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 7);
    }

    BOOST_AUTO_TEST_CASE(testComments3) {
        string code = "a = 1 /* *** this is not closed \n \t comment";
        Lexer l(code);
        BOOST_CHECK(!l.run());
        auto errors = l.getErrors();
        BOOST_CHECK_EQUAL(errors.size(), 1);
        BOOST_CHECK_EQUAL(errors[0].first, 0);
        BOOST_CHECK_EQUAL(errors[0].second, 6);
    }

    BOOST_AUTO_TEST_CASE(testComments4) {
        string code = R"foo(
a/* *** this comment ****/ b // "this is comment too"
c // comment      d e f // * */ /*
d
        )foo";
        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();
        BOOST_CHECK_EQUAL(atoms.size(), 4 + 1);
        BOOST_CHECK_EQUAL("a",atoms[0]->getRepr());
        BOOST_CHECK_EQUAL("b",atoms[1]->getRepr());
        BOOST_CHECK_EQUAL("c",atoms[2]->getRepr());
        BOOST_CHECK_EQUAL("d",atoms[3]->getRepr());
        BOOST_CHECK_EQUAL("$",atoms[4]->getRepr());
    }

    BOOST_AUTO_TEST_CASE(testGetAtom1) {
        string code = R"foo(
for (var mi = -824 + 824; mi < arenderer[jradtodeg(idoxie) + imsg + iparticleradius]; mi++)
{
    atends = arenderer[mi];
    if (atends < (-1909 + 2037)) 
        {renderer = atends;}
    else 
        {renderer = othumbnailslist[atends];}
    aflexwhitespace.push(String.fromCharCode(renderer));
}
)foo";

        Lexer l(code);
        BOOST_CHECK(l.run());
        auto atoms = l.getAtoms();

        Lexer l2(code);

        Atom* newAtom = l2.getNextAtom();
        int i=0;
        while (newAtom->getRepr() != "$") {
            BOOST_CHECK_EQUAL(newAtom->getRepr(), atoms[i]->getRepr());
            ++i;
            newAtom = l2.getNextAtom();
        }

        BOOST_CHECK_THROW(l2.getNextAtom(), string);
    }

    BOOST_AUTO_TEST_CASE(testGetAtom2) {
        string code = "var a=1";
        Lexer l(code);

        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "var");
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "a");
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "=");
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "1");
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "$");

        BOOST_CHECK_THROW(l.getNextAtom(), string);
    }

    BOOST_AUTO_TEST_CASE(testGetLastError) {
        string code = "a = 1a 4 1.g";
        Lexer l(code);

        // a
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "a");
        BOOST_CHECK_THROW(l.getLastError(), string);

        // =
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "=");
        BOOST_CHECK_THROW(l.getLastError(), string);

        // 1a
        BOOST_CHECK_EQUAL(l.getNextAtom(), (void*)nullptr);
        std::pair<unsigned,unsigned> p = std::pair<unsigned,unsigned>({0,4});
        BOOST_CHECK(l.getLastError() == p);
        BOOST_CHECK(l.getLastError() == p);

        // 4
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "4");
        BOOST_CHECK(l.getLastError() == p);

        // 1.g
        BOOST_CHECK_EQUAL(l.getNextAtom(), (void*)nullptr);
        std::pair<unsigned,unsigned> p2 = std::pair<unsigned,unsigned>({0,9});
        BOOST_CHECK(l.getLastError() == p2);

        // $
        BOOST_CHECK_EQUAL(l.getNextAtom()->getRepr(), "$");
        BOOST_CHECK(l.getLastError() == p2);

        BOOST_CHECK_THROW(l.getNextAtom(), string);
    }
}
