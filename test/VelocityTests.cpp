#include <boost/test/unit_test.hpp>

#include "Velocity.hpp"

namespace Model
{

BOOST_AUTO_TEST_SUITE(CollisionManagerTests)

BOOST_AUTO_TEST_CASE(ShouldCreateProperly)
{
    Velocity vel1;
    BOOST_CHECK_EQUAL(vel1.x, 0);
    BOOST_CHECK_EQUAL(vel1.y, 0);

    Velocity vel2{1,3};
    BOOST_CHECK_EQUAL(vel2.x, 1);
    BOOST_CHECK_EQUAL(vel2.y, 3);

    Velocity vel3 = oxygine::Vector2(5, 7);
    BOOST_CHECK_EQUAL(vel3.x, 5);
    BOOST_CHECK_EQUAL(vel3.y, 7);

    Velocity vel4 = vel2;
    BOOST_CHECK_EQUAL(vel4.x, 1);
    BOOST_CHECK_EQUAL(vel4.y, 3);

}

BOOST_AUTO_TEST_SUITE_END()

}
