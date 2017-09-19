#pragma once

#include "math/Vector2.h"

#include "Utils.hpp"

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
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
};

struct Ray
{
    Ray(const Service::Vector2L& a_Original, const Service::Vector2L& a_Destination, RayDirection a_Direction)
        : Original(a_Original)
        , Destination(a_Destination)
        , Direction(a_Direction)
    {}

    // Change to Point (VectorT2<int> - don't need such precise, no to forget change in other places!!!).
    Service::Vector2L Original;
    Service::Vector2L Destination;
    RayDirection Direction;
};

}
