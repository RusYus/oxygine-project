#pragma once

#include "math/Vector2.h"

#include "Utils.hpp"

namespace Collision
{

struct CollisionInfo
{
public:
    CollisionInfo() = default;

    CollisionInfo(bool a_Up, bool a_Down, bool a_Right, bool a_Left)
        : Up(a_Up)
        , Down(a_Down)
        , Right(a_Right)
        , Left(a_Left)
    {}

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

static const CollisionInfo CollisionInfoUp{true, false, false, false};
static const CollisionInfo CollisionInfoDown{false, true, false, false};
static const CollisionInfo CollisionInfoRight{false, false, true, false};
static const CollisionInfo CollisionInfoLeft{false, false, false, true};

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

    Service::Vector2L Original;
    Service::Vector2L Destination;
    RayDirection Direction;
};

}
