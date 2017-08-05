#pragma once

#include "oxygine-framework.h"
#include "Constants.hpp"
#include "Box2D/Box2D.h"

enum class RayDirection
{
    Up,
    Down,
    Left,
    Right,
};

struct Ray
{
    Ray(const oxygine::Vector2& aOriginal, const oxygine::Vector2& aDestination, RayDirection aDirection)
        : Original(aOriginal)
        , Destination(aDestination)
        , IsHitInLastStep(false)
        , IsHitInCurrentStep(false)
        , Direction(aDirection)
    {}

    // Change to Point (VectorT2<int> - don't need such precise, no to forget change in other places!!!).
    oxygine::Vector2 Original;
    oxygine::Vector2 Destination;
    bool IsHitInLastStep;
    bool IsHitInCurrentStep;
    RayDirection Direction;
};

namespace Service
{

struct Utils
{
    static b2Vec2 Convert(const oxygine::Vector2& aPos)
    {
        return b2Vec2(aPos.x / Constants::SCALE, aPos.y / Constants::SCALE);
    }

    static oxygine::Vector2 Convert(const b2Vec2& aPos)
    {
        return oxygine::Vector2(aPos.x * Constants::SCALE, aPos.y * Constants::SCALE);
    }
};

class IdGenerator
{
public:
    static inline int GetNextId()
    {
        return ++mId;
    }

private:
    static int mId;
};

struct Normal2: b2Vec2
{
    Normal2() = default;

    Normal2(float32 x, float32 y)
        : b2Vec2(x, y)
    {
    }

    void operator =(const oxygine::Vector2& aVector)
    {
        x = aVector.x;
        y = aVector.y;
    }

    void operator += (const oxygine::Vector2& aVector)
    {
        x += aVector.x;
        y += aVector.y;
        FixCoordinatesIfExceeds();
    }

    void operator += (const Normal2& aVector)
    {
        x += aVector.x;
        y += aVector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const oxygine::Vector2& aVector)
    {
        x -= aVector.x;
        y -= aVector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const Normal2& aVector)
    {
        x -= aVector.x;
        y -= aVector.y;
        FixCoordinatesIfExceeds();
    }

    bool operator == (const Normal2& aVector)
    {
        return x == aVector.x && y == aVector.y;
    }

    bool operator != (const Normal2& aVector)
    {
        return !this->operator ==(aVector);
    }

    inline void FixCoordinatesIfExceeds()
    {
        if (x < -1)
        {
            x = -1;
        }

        if (x > 1)
        {
            x = 1;
        }

        if (y < -1)
        {
            y = -1;
        }

        if (y > 1)
        {
            y = 1;
        }
    }
};

static const Normal2 ZeroNormal = Normal2{0, 0};

}
