#include <boost/test/unit_test.hpp>

#include "Velocity.hpp"
#include "Utils.hpp"

namespace Model
{

BOOST_AUTO_TEST_SUITE(VelocityTests)

BOOST_AUTO_TEST_CASE(CtorsTest)
{
    Velocity vel1;
    BOOST_CHECK(Service::AreEqual(vel1.x, 0));
    BOOST_CHECK(Service::AreEqual(vel1.y, 0));

    Velocity vel2{1.1,3.3};
    BOOST_CHECK(Service::AreEqual(vel2.x, 1.1));
    BOOST_CHECK(Service::AreEqual(vel2.y, 3.3));

    Velocity vel3 = oxygine::Vector2(5.5, 7.7);
    BOOST_CHECK(Service::AreEqual(vel3.x, 5.5));
    BOOST_CHECK(Service::AreEqual(vel3.y, 7.7));

    Velocity vel4 = vel2;
    BOOST_CHECK(Service::AreEqual(vel4.x, 1.1));
    BOOST_CHECK(Service::AreEqual(vel4.y, 3.3));
}

BOOST_AUTO_TEST_CASE(AdditionTest)
{
    Velocity v(1.1, 2.2);

    Velocity res = v + 3.3;
    BOOST_CHECK(Service::AreEqual(res.x, 4.4));
    BOOST_CHECK(Service::AreEqual(res.y, 5.5));

    res = v + oxygine::Vector2(1.2, 2.3);
    BOOST_CHECK(Service::AreEqual(res.x, 2.3));
    BOOST_CHECK(Service::AreEqual(res.y, 4.5));

    res = oxygine::Vector2(1.6, 2.1) + v;
    BOOST_CHECK(Service::AreEqual(res.x, 2.7));
    BOOST_CHECK(Service::AreEqual(res.y, 4.3));

    res = v + Velocity(2.1, 3.9);
    BOOST_CHECK(Service::AreEqual(res.x, 3.2));
    BOOST_CHECK(Service::AreEqual(res.y, 6.1));

    res = Velocity(2.6, 6.1) + v;
    BOOST_CHECK(Service::AreEqual(res.x, 3.7));
    BOOST_CHECK(Service::AreEqual(res.y, 8.3));

    // +=
    res = v;

    res += 3.3;
    BOOST_CHECK(Service::AreEqual(res.x, 4.4));
    BOOST_CHECK(Service::AreEqual(res.y, 5.5));

    res += oxygine::Vector2(1.2, 2.3);
    BOOST_CHECK(Service::AreEqual(res.x, 5.6));
    BOOST_CHECK(Service::AreEqual(res.y, 7.8));

    res += Velocity(2.1, 3.9);
    BOOST_CHECK(Service::AreEqual(res.x, 7.7));
    BOOST_CHECK(Service::AreEqual(res.y, 11.7));
}

BOOST_AUTO_TEST_CASE(SubstractionTest)
{
    Velocity v(1.1, 2.2);

    Velocity res = v - 3.3;
    BOOST_CHECK(Service::AreEqual(res.x, -2.2));
    BOOST_CHECK(Service::AreEqual(res.y, -1.1));

    res = v - oxygine::Vector2(1.2, 2.4);
    BOOST_CHECK(Service::AreEqual(res.x, -0.1));
    BOOST_CHECK(Service::AreEqual(res.y, -0.2));

    res = oxygine::Vector2(1.6, 2.1) - v;
    BOOST_CHECK(Service::AreEqual(res.x, 0.5));
    BOOST_CHECK(Service::AreEqual(res.y, -0.1));

    res = v - Velocity(2.1, 3.9);
    BOOST_CHECK(Service::AreEqual(res.x, -1.0));
    BOOST_CHECK(Service::AreEqual(res.y, -1.7));

    res = Velocity(2.6, 6.1) - v;
    BOOST_CHECK(Service::AreEqual(res.x, 1.5));
    BOOST_CHECK(Service::AreEqual(res.y, 3.9));

    // -=
    res = v;

    res -= 3.3;
    BOOST_CHECK(Service::AreEqual(res.x, -2.2));
    BOOST_CHECK(Service::AreEqual(res.y, -1.1));

    res -= oxygine::Vector2(1.2, 3.3);
    BOOST_CHECK(Service::AreEqual(res.x, -3.4));
    BOOST_CHECK(Service::AreEqual(res.y, -4.4));

    res -= Velocity(2.1, 3.9);
    BOOST_CHECK(Service::AreEqual(res.x, -5.5));
    BOOST_CHECK(Service::AreEqual(res.y, -8.3));
}

BOOST_AUTO_TEST_CASE(MultiplicationTest)
{
    Velocity v(1, 2);

    Velocity res = v * 3;
    BOOST_CHECK(Service::AreEqual(res.x, 3));
    BOOST_CHECK(Service::AreEqual(res.y, 6));

    res = v * oxygine::Vector2(2, 4);
    BOOST_CHECK(Service::AreEqual(res.x, 2));
    BOOST_CHECK(Service::AreEqual(res.y, 8));

    res = oxygine::Vector2(5, 2) * v;
    BOOST_CHECK(Service::AreEqual(res.x, 5));
    BOOST_CHECK(Service::AreEqual(res.y, 4));

    res = v * Velocity(3, 4);
    BOOST_CHECK(Service::AreEqual(res.x, 3));
    BOOST_CHECK(Service::AreEqual(res.y, 8));

    res = Velocity(4, 6) * v;
    BOOST_CHECK(Service::AreEqual(res.x, 4));
    BOOST_CHECK(Service::AreEqual(res.y, 12));

    // *=
    res = v;

    res *= 7;
    BOOST_CHECK(Service::AreEqual(res.x, 7));
    BOOST_CHECK(Service::AreEqual(res.y, 14));

    res *= oxygine::Vector2(2, 3);
    BOOST_CHECK(Service::AreEqual(res.x, 14));
    BOOST_CHECK(Service::AreEqual(res.y, 42));

    res *= Velocity(1, 4);
    BOOST_CHECK(Service::AreEqual(res.x, 14));
    BOOST_CHECK(Service::AreEqual(res.y, 168));
}

BOOST_AUTO_TEST_CASE(DivisionTest)
{
    Velocity v(600, 256);

    Velocity res = v / 2;
    BOOST_CHECK(Service::AreEqual(res.x, 300));
    BOOST_CHECK(Service::AreEqual(res.y, 128));

    res = v / oxygine::Vector2(3, 1);
    BOOST_CHECK(Service::AreEqual(res.x, 200));
    BOOST_CHECK(Service::AreEqual(res.y, 256));

    res = oxygine::Vector2(1200, 1024) / v;
    BOOST_CHECK(Service::AreEqual(res.x, 2));
    BOOST_CHECK(Service::AreEqual(res.y, 4));

    res = v / Velocity(60, 8);
    BOOST_CHECK(Service::AreEqual(res.x, 10));
    BOOST_CHECK(Service::AreEqual(res.y, 32));

    res = Velocity(600, 512) / v;
    BOOST_CHECK(Service::AreEqual(res.x, 1));
    BOOST_CHECK(Service::AreEqual(res.y, 2));

    // /=
    res = v;

    res /= 4;
    BOOST_CHECK(Service::AreEqual(res.x, 150));
    BOOST_CHECK(Service::AreEqual(res.y, 64));

    res /= oxygine::Vector2(50, 8);
    BOOST_CHECK(Service::AreEqual(res.x, 3));
    BOOST_CHECK(Service::AreEqual(res.y, 8));

    res /= Velocity(3, 4);
    BOOST_CHECK(Service::AreEqual(res.x, 1));
    BOOST_CHECK(Service::AreEqual(res.y, 2));
}

BOOST_AUTO_TEST_SUITE_END()

}
