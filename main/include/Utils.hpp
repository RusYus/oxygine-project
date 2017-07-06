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

struct Normal2: b2Vec2
{
    Normal2(float32 x, float32 y)
        : b2Vec2(x, y)
    {
    }

    /// Add a vector to this vector.
    void operator += (const b2Vec2& aVector)
    {
        x += aVector.x;
        y += aVector.y;
        FixCoordinatesIfExceeds();
    }

    /// Add a vector to this vector.
    void operator += (const Normal2& aVector)
    {
        x += aVector.x;
        y += aVector.y;
        FixCoordinatesIfExceeds();
    }

    /// Add a vector to this vector.
    void operator -= (const b2Vec2& aVector)
    {
        x -= aVector.x;
        y -= aVector.y;
        FixCoordinatesIfExceeds();
    }

    /// Add a vector to this vector.
    void operator -= (const Normal2& aVector)
    {
        x -= aVector.x;
        y -= aVector.y;
        FixCoordinatesIfExceeds();
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

}
