#pragma once

#include "oxygine-framework.h"
#include "Constants.hpp"

namespace Service
{

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

struct Normal2: oxygine::Vector2
{
    Normal2() = default;

    Normal2(float x, float y)
        : oxygine::Vector2(x, y)
    {
    }

    void operator =(const oxygine::Vector2& a_Vector)
    {
        x = a_Vector.x;
        y = a_Vector.y;
    }

    void operator += (const oxygine::Vector2& a_Vector)
    {
        x += a_Vector.x;
        y += a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator += (const Normal2& a_Vector)
    {
        x += a_Vector.x;
        y += a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const oxygine::Vector2& a_Vector)
    {
        x -= a_Vector.x;
        y -= a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const Normal2& a_Vector)
    {
        x -= a_Vector.x;
        y -= a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    bool operator == (const Normal2& a_Vector)
    {
        return x == a_Vector.x && y == a_Vector.y;
    }

    bool operator != (const Normal2& a_Vector)
    {
        return !this->operator ==(a_Vector);
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
