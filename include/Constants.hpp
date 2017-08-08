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
    // Scaling factor between project and box2d.
    static constexpr const float SCALE = 100.0f;
    static constexpr const int GRAVITY = 50;
    static constexpr const int MAX_SPEED = 5;
    static constexpr const int PLAYER_MAX_SPEED = 5;
    static constexpr const int PLAYER_JUMP_SPEED = 10;
    static constexpr const int RAYCAST_INTERVAL = 25;
};

}
