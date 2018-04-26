#include <boost/test/unit_test.hpp>

#include "CollisionManager.hpp"
#include "DynamicBoxFake.hpp"
#include "Ground.hpp"
#include "PlayerFake.hpp"
#include "PlatformFake.hpp"

namespace Service
{

BOOST_AUTO_TEST_SUITE(CollisionManagerTests)

struct CollisionManagerFixture
{
    std::shared_ptr<CollisionManager> m_Manager;
    std::unique_ptr<PlayerFake> m_Player;
    std::unique_ptr<Static> m_Ground;
    std::unique_ptr<PlatformFake> m_Platform;
    std::unique_ptr<DynamicBoxFake> m_DynamicBox;
    static const constexpr int m_Distance = 1'000;
    static const constexpr int m_Threshold = 500;

    CollisionManagerFixture()
        : m_Manager(std::make_shared<CollisionManager>())
        , m_Player(std::make_unique<PlayerFake>(m_Manager))
        , m_Ground(std::make_unique<Static>(oxygine::Rect(30'000, 10'000, 20'000, 5'000)))
        , m_Platform(std::make_unique<PlatformFake>(oxygine::Rect(1, 1, 1, 1), m_Manager))
        , m_DynamicBox(std::make_unique<DynamicBoxFake>(oxygine::Rect(1, 1, 1, 1), m_Manager))
    {
        static_assert(m_Distance > m_Threshold, "Distance should be bigger than threshold!");
        m_Manager->AddBody(m_Ground.get());
    }
};

/*BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOutsideDestinationInsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth() - m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(m_Distance + m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() + m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(-m_Distance - m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight() - m_Distance,
        Service::Vector2L(0, m_Distance + m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() + m_Ground->GetHeight() + m_Distance,
        Service::Vector2L(0, -m_Distance - m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOutsideDestinationInsideAngled, CollisionManagerFixture)
{
    // Left side
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth() - m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(m_Distance + m_Threshold, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Right side
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() + m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(-m_Distance - m_Threshold, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Up side
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight() - m_Distance,
        Service::Vector2L(m_Threshold, m_Distance + m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down side
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() + m_Ground->GetHeight() + m_Distance,
        Service::Vector2L(-m_Threshold, -m_Distance - m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldNotIntersectWhenOriginalOutsideDestinationOnBorderAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth() - m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(m_Distance, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() + m_Distance,
        m_Ground->GetY(),
        Service::Vector2L(-m_Distance, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight() - m_Distance,
        Service::Vector2L(0, m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() + m_Ground->GetHeight() + m_Distance,
        Service::Vector2L(0, -m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenOriginalOnBorderDestinationInsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(-m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight(),
        Service::Vector2L(0, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() + m_Ground->GetHeight(),
        Service::Vector2L(0, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldNotIntersectWhenOriginalOnBorderDestinationOutsideAxisAlligned, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(-m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight(),
        Service::Vector2L(0, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() + m_Ground->GetHeight(),
        Service::Vector2L(0, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldNotIntersectWhenMovingOnSurface, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(0, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth(),
        m_Ground->GetY() + m_Ground->GetHeight(),
        Service::Vector2L(0, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight(),
        Service::Vector2L(m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth(),
        m_Ground->GetY() + m_Ground->GetHeight(),
        Service::Vector2L(-m_Threshold, 0));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(!m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenMovingOnSurfaceWithForceTowardsIt, CollisionManagerFixture)
{
    // Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth(),
        m_Ground->GetY(),
        Service::Vector2L(m_Threshold, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth(),
        m_Ground->GetY() + m_Ground->GetHeight() - m_Player->GetHeight(),
        Service::Vector2L(-m_Threshold, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Up side.
    m_Player->SetupValues(
        m_Ground->GetX(),
        m_Ground->GetY() - m_Player->GetHeight(),
        Service::Vector2L(m_Threshold, m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() - m_Player->GetWidth(),
        m_Ground->GetY() + m_Ground->GetHeight(),
        Service::Vector2L(-m_Threshold, -m_Threshold));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(ShouldIntersectWhenMovingOnTheCorner, CollisionManagerFixture)
{
    // Up Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth() - m_Threshold,
        m_Ground->GetY() - m_Player->GetHeight() - m_Threshold,
        Service::Vector2L(m_Distance, m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Up Right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() + m_Threshold,
        m_Ground->GetY() - m_Player->GetHeight() - m_Threshold,
        Service::Vector2L(-m_Distance, m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down Left side.
    m_Player->SetupValues(
        m_Ground->GetX() - m_Player->GetWidth() - m_Threshold,
        m_Ground->GetY() + m_Ground->GetHeight() + m_Threshold,
        Service::Vector2L(m_Distance, -m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());

    // Down right side.
    m_Player->SetupValues(
        m_Ground->GetX() + m_Ground->GetWidth() + m_Threshold,
        m_Ground->GetY() + m_Ground->GetHeight() + m_Threshold,
        Service::Vector2L(-m_Distance, -m_Distance));
    m_Manager->CheckCollisions(m_Player->GetId());
    BOOST_CHECK(m_Player->CollisionTookPlace());
}

BOOST_FIXTURE_TEST_CASE(PlayerShouldNotMoveDynamicBoxWhenBothOnPlatform, CollisionManagerFixture)
{
    m_Platform->SetupValues(50'000, 20'000, {-300, 0});
    m_Player->SetupValues(50'000, 10'000, {500, 100});
    m_DynamicBox->SetupValues(60'100, 10'000, {0, 100});
    m_Platform->Update();
    m_DynamicBox->Update();
    m_Player->CheckCollisions();
    m_DynamicBox->SetPosition();
    m_Player->SetPosition();
    BOOST_CHECK(m_Player->GetX() == 50'200);
    BOOST_CHECK(m_Player->GetY() == 10'000);
}*/

BOOST_AUTO_TEST_SUITE_END()

}

