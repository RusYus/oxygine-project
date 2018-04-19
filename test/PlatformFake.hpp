#include <boost/test/unit_test.hpp>

#include "Actor.h"

#include "ICarrier.hpp"

struct PlatformFake : public ICarrier
{
public:
    PlatformFake(const oxygine::Rect&, const std::shared_ptr<ICollisionManager>&);

    void SetupValues(int /*a_X*/, int /*a_Y*/, const Service::Vector2L& /*a_Direction*/);

    inline Service::TCoordinate GetWidth() const override
    {
        return Service::TCoordinate(25'000);
    }

    inline Service::TCoordinate GetHeight() const override
    {
        return Service::TCoordinate(2'000);
    }

    void Update();

public:
    bool m_WasCollision;
    Service::Vector2L m_InitialDirection;
};
