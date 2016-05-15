#include "../../src/util/buf.h"

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

BOOST_AUTO_TEST_SUITE(TestBuffer)

BOOST_AUTO_TEST_CASE(blackman) {
    Buffer<double> buf("blackman");
    buf.setLimit(101);
    buf.blackman();
    BOOST_REQUIRE_SMALL(buf.at(0), 1e-16);
    BOOST_REQUIRE_SMALL(buf.at(1), 1e-3);
    BOOST_REQUIRE_CLOSE_FRACTION(1, buf.at(49), 1e-2);
    BOOST_REQUIRE_CLOSE_FRACTION(1, buf.at(50), 1e-15);
    BOOST_REQUIRE_CLOSE_FRACTION(1, buf.at(51), 1e-2);
    BOOST_REQUIRE_SMALL(buf.at(99), 1e-3);
    BOOST_REQUIRE_SMALL(buf.at(100), 1e-16);
    BOOST_REQUIRE_LT(buf.at(0), buf.at(1));
    BOOST_REQUIRE_LT(buf.at(49), buf.at(50));
    BOOST_REQUIRE_GT(buf.at(50), buf.at(51));
    BOOST_REQUIRE_GT(buf.at(99), buf.at(100));
}

BOOST_AUTO_TEST_SUITE_END()