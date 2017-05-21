#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestExample)

struct TestFixture
{
};

BOOST_FIXTURE_TEST_CASE(TestCase, TestFixture)
{
    BOOST_CHECK_EQUAL(1+3, 4);
}

BOOST_AUTO_TEST_SUITE_END()
