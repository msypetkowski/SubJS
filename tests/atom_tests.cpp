#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include <boost/range/combine.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <array>

// #define private public
// #define protected public

#include "../src/atom.h"

namespace AtomTests
{
    BOOST_AUTO_TEST_CASE(testSymset1) {
        SymSet ss({"=", ","});
        BOOST_CHECK(!ss.hasSymbol());
        BOOST_CHECK(!ss.hasConstant());
        BOOST_CHECK(!ss.hasKeyword(";"));
        BOOST_CHECK(ss.hasKeyword("="));
        BOOST_CHECK(ss.hasKeyword(","));
        BOOST_CHECK(!ss.hasKeyword("$"));
        BOOST_CHECK_EQUAL(2,ss.size());
        ss.includeSymbol();
        BOOST_CHECK_EQUAL(3,ss.size());
        BOOST_CHECK(ss.hasSymbol());
        ss.includeSymbol();
        BOOST_CHECK_EQUAL(3,ss.size());
        BOOST_CHECK(ss.hasSymbol());
        BOOST_CHECK(!ss.hasConstant());
    }

    BOOST_AUTO_TEST_CASE(testSymsetAdd) {
        SymSet ss1({"=", ","});
        ss1.includeConstant();
        SymSet ss2({"var", ","});

        BOOST_CHECK_EQUAL(4,(ss1 + ss2).size());
    }
}
