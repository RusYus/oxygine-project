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
};

}
