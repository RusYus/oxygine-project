#pragma once

#include "oxygine-framework.h"
#include "Constants.hpp"
#include "Box2D/Box2D.h"

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
    using TId = int;
    static inline TId GetNextId()
    {
        return ++mId;
    }

private:
    static TId mId;
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

float RoundToOneDigit(float);

void RoundToOneDigit(oxygine::Vector2&);

}
