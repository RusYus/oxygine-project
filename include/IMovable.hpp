#pragma once

#include <vector>

#include "math/Vector2.h"

#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

class IMovable
{
private:
    oxygine::Vector2 mDirection;
    Service::Normal2 mCollisionNormal;
    std::vector<Collision::Ray> mRays;
    const int mMaxSpeed = Service::Constants::PLAYER_MAX_SPEED;
};
