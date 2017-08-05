#include <boost/test/unit_test.hpp>

#include "CollisionInfo.hpp"
#include "CollisionManager.hpp"

namespace Service
{

BOOST_AUTO_TEST_SUITE(CollisionManagerTests)

struct CollisionManagerFixture
{
    class TestAabb
    {
    public:
        TestAabb(const oxygine::Vector2& a_Coordinates, int a_Width, int a_Height)
            : m_Coordinates(a_Coordinates)
            , m_Width(a_Width)
            , m_Height(a_Height)
        {
        }

        TestAabb& SetCoordinates(const oxygine::Vector2& a_Coordinates)
        {
            m_Coordinates = a_Coordinates;
            return *this;
        }

        TestAabb& SetWidth(int a_Width)
        {
            m_Width = a_Width;
            return *this;
        }

        TestAabb& SetHeight(int a_Height)
        {
            m_Height = a_Height;
            return *this;
        }

        oxygine::Vector2 GetBottomLeft() const
        {
            return oxygine::Vector2(m_Coordinates.x, m_Coordinates.y + m_Height);
        }

        oxygine::Vector2 GetTopRight() const
        {
            return oxygine::Vector2(m_Coordinates.x + m_Width, m_Coordinates.y);
        }

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }

        TestAabb Build()
        {
            return TestAabb(m_Coordinates, m_Width, m_Height);
        }

    private:
        oxygine::Vector2 m_Coordinates;
        int m_Width;
        int m_Height;
    };

    class TestRay
    {
    public:
        TestRay(const oxygine::Vector2& a_Original, const oxygine::Vector2& a_Destination, Collision::RayDirection a_Direction)
            : m_Original(a_Original)
            , m_Destination(a_Destination)
            , m_Direction(a_Direction)
        {}

        TestRay& SetOriginal(int a_X, int a_Y)
        {
            m_Original = oxygine::Vector2(a_X, a_Y);
            return *this;
        }

        TestRay& SetDestination(int a_X, int a_Y)
        {
            m_Destination = oxygine::Vector2(a_X, a_Y);
            return *this;
        }

        TestRay& SetDirection(Collision::RayDirection a_Direction)
        {
            m_Direction = a_Direction;
            return *this;
        }

        oxygine::Vector2 GetOriginal() const
        {
            return m_Original;
        }

        oxygine::Vector2 GetDestination() const
        {
            return m_Destination;
        }

        Collision::RayDirection GetDirection() const
        {
            return m_Direction;
        }

        Collision::Ray Build()
        {
            return Collision::Ray(m_Original, m_Destination, m_Direction);
        }

    private:
        oxygine::Vector2 m_Original;
        oxygine::Vector2 m_Destination;
        Collision::RayDirection m_Direction;
    };

    TestAabb Aabb;
    TestRay Ray;
    oxygine::Vector2 OutIntersection;
    CollisionManager Manager;

    CollisionManagerFixture()
        : Aabb(oxygine::Vector2(100, 50), 30, 20)
        , Ray(oxygine::Vector2(90, 60), oxygine::Vector2(110, 60), Collision::RayDirection::Right)
    {}
};

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOutsideDestinationInsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(200, 60);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(110, 80);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(110, 40);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOutsideDestinationInsideAngled, CollisionManagerFixture)
{
    // Left side.
    Ray = Ray.SetDestination(110, 50);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(200, 30);
    Ray = Ray.SetDestination(110, 70);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(50, 80);
    Ray = Ray.SetDestination(120, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(0, 40);
    Ray = Ray.SetDestination(150, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOutsideDestinationOnBorderAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    Ray = Ray.SetDestination(100, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(200, 60);
    Ray = Ray.SetDestination(130, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(110, 80);
    Ray = Ray.SetDestination(110, 70);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(110, 40);
    Ray = Ray.SetDestination(110, 50);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOnBorderDestinationInsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    Ray = Ray.SetOriginal(100, 60);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(130, 60);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(110, 50);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(110, 70);
    Ray = Ray.SetDestination(110, 60);
    BOOST_CHECK(Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_FIXTURE_TEST_CASE(ShouldNotIntersectWhenOriginalOnBorderDestinationOutsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    Ray = Ray.SetOriginal(100, 60);
    Ray = Ray.SetDestination(80, 60);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(130, 60);
    Ray = Ray.SetDestination(150, 60);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(110, 50);
    Ray = Ray.SetDestination(110, 30);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(110, 70);
    Ray = Ray.SetDestination(110, 90);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_FIXTURE_TEST_CASE(ShouldNotIntersectWhenOriginalOnBorderDestinationOnBorderAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    Ray = Ray.SetOriginal(100, 50);
    Ray = Ray.SetDestination(100, 60);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Right side.
    Ray = Ray.SetOriginal(130, 60);
    Ray = Ray.SetDestination(130, 50);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Up side.
    Ray = Ray.SetOriginal(110, 50);
    Ray = Ray.SetDestination(120, 50);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
    // Down side.
    Ray = Ray.SetOriginal(120, 70);
    Ray = Ray.SetDestination(110, 70);
    BOOST_CHECK(!Manager.Intersection(Aabb.GetBottomLeft(), Aabb.GetTopRight(), Ray.GetOriginal(), Ray.GetDestination(), OutIntersection));
}

BOOST_AUTO_TEST_SUITE_END()

}

