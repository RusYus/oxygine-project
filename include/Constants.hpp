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
    static constexpr const int SCALE = 100;
    static constexpr const int GRAVITY = 100;
    static constexpr const int MAX_SPEED = 3 * SCALE;
    static constexpr const int PLAYER_MAX_SPEED = 5 * SCALE;
    static constexpr const int PLAYER_JUMP_SPEED = 1600;
    static constexpr const int RAYCAST_INTERVAL = 25 * SCALE;
    static constexpr const int CARRIER_RAY_LENGTH = 1 * SCALE;
    static constexpr const float THRESHOLD = 0.1;
    static constexpr const float EPSILON = 0.00001;
};

}
