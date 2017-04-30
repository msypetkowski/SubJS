#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include <boost/range/combine.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <array>

// #define private public
// #define protected public

#include "../src/atom.h"
#include "../src/lexer.h"
#include "../src/parser.h"

namespace ParserTests
{
    BOOST_AUTO_TEST_CASE(testDeclaration1) {
        string code = "var a=123";
        Lexer l(code);
        Parser p(l);
        p.Program();

        BOOST_CHECK(p.getErrorsPositions().empty());
        BOOST_CHECK(p.getErrorsMessages().empty());

        string expectedTreeStr=R"(
Program
.Declaration
..DeclarationType
...Keyword 15 : var
..DeclarationElem
...Symbol 0 : a
...Keyword 35 : =
...Constant Integer: 123
        )";
        string producedStr = p.getTreeStr();

        boost::trim(expectedTreeStr);
        boost::trim(producedStr);
        BOOST_CHECK_EQUAL(expectedTreeStr,producedStr);
    }
    BOOST_AUTO_TEST_CASE(testDeclaration2) {
        string code = "var qwe=4,b,c=1";
        Lexer l(code);
        Parser p(l);
        p.Program();

        BOOST_CHECK(p.getErrorsPositions().empty());
        BOOST_CHECK(p.getErrorsMessages().empty());

        string expectedTreeStr=R"(
Program
.Declaration
..DeclarationType
...Keyword 15 : var
..DeclarationElem
...Symbol 0 : qwe
...Keyword 35 : =
...Constant Integer: 4
..Keyword 3 : ,
..DeclarationElem
...Symbol 1 : b
..Keyword 3 : ,
..DeclarationElem
...Symbol 2 : c
...Keyword 35 : =
...Constant Integer: 1
        )";
        string producedStr = p.getTreeStr();

        boost::trim(expectedTreeStr);
        boost::trim(producedStr);
        BOOST_CHECK_EQUAL(expectedTreeStr,producedStr);
    }
}
