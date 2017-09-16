#pragma once

namespace Service
{

enum class ObjectType
{
    Player,
    DynamicBody,
    Ground,
    UNDEFINED,
};

struct Constants
{
    static constexpr const int GRAVITY = 50;
    static constexpr const int MAX_SPEED = 3;
    static constexpr const int PLAYER_MAX_SPEED = 5;
    static constexpr const int PLAYER_JUMP_SPEED = 10;
    static constexpr const int RAYCAST_INTERVAL = 25;
    static constexpr const int CARRIER_RAY_LENGTH = 1;
    static constexpr const float THRESHOLD = 0.1;
    static constexpr const float EPSILON = 0.00001;
};

}
