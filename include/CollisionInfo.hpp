#pragma once

#include "math/Vector2.h"

namespace Collision
{

struct CollisionInfo
{
public:
    inline void Reset()
    {
        Up = false;
        Down = false;
        Right = false;
        Left = false;
    }

    bool Up = false;
    bool Down = false;
    bool Right = false;
    bool Left = false;
};

enum class RayDirection
{
    Up,
    Down,
    Left,
    Right,
};

struct Ray
{
    Ray(const oxygine::Vector2& a_Original, const oxygine::Vector2& a_Destination, RayDirection a_Direction)
        : Original(a_Original)
        , Destination(a_Destination)
        , Direction(a_Direction)
    {}

    // Change to Point (VectorT2<int> - don't need such precise, no to forget change in other places!!!).
    oxygine::Vector2 Original;
    oxygine::Vector2 Destination;
    RayDirection Direction;
};

}
