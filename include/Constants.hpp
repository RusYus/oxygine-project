#pragma once

#include <iostream>

namespace Service
{

enum class ObjectType
{
    Player,
    Platform,
    DynamicBody,
    StaticBody,
    UNDEFINED,
};

inline std::ostream& operator << (std::ostream& out_Stream, ObjectType a_ObjectType)
{
        switch (a_ObjectType)
        {
        case ObjectType::Player:
            return out_Stream << "Player";
        case ObjectType::Platform:
            return out_Stream << "Platform";
        case ObjectType::DynamicBody:
            return out_Stream << "DynamicBody";
        case ObjectType::StaticBody:
            return out_Stream << "StaticBody";
        case ObjectType::UNDEFINED:
            return out_Stream << "UNDEFINED";
        default:
            return out_Stream << "???";
        }
}

struct Constants
{
    static constexpr const int SCALE = 100;
    static constexpr const int GRAVITY = 100;
    static constexpr const int MAX_SPEED = 3 * SCALE;
    static constexpr const int PLAYER_MAX_SPEED = 5 * SCALE;
//    static constexpr const int PLAYER_JUMP_SPEED = 1600;
    static constexpr const int PLAYER_JUMP_SPEED = 2000;
    static constexpr const int RAYCAST_INTERVAL = 25 * SCALE;
    static constexpr const int CARRIER_RAY_LENGTH = 1 * SCALE;
    static constexpr const float THRESHOLD = 0.1;
    static constexpr const float EPSILON = 0.00001;
};

}
