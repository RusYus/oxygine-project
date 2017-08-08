#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Dynamics/b2Body.h"

enum class PointToPointMode
{
    Cycle,
    BackToBack,
};

struct PathNode
{
    using TId = unsigned short;

    PathNode(unsigned short aId, const oxygine::Vector2& aPosition)
    {
        Id = aId;
        Position = aPosition;
    }

    TId Id;
    oxygine::Vector2 Position;
};

static const constexpr float32 NODE_SLIPPAGE = 0.08;

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Box9Sprite, public Basis::BasisObject
{
public:
    Platform(const oxygine::RectF&);
    void Move();

private:
    bool IsAroundNode();

private:
    PointToPointMode m_RunningMode = PointToPointMode::BackToBack;
    typename PathNode::TId m_NextNodeId = 1;
    bool m_IsMovingReverse = false;
    oxygine::Vector2 m_Direction;
    const int m_Speed = 3;
    std::unordered_map<PathNode::TId, PathNode> m_Nodes;
    std::pair<Service::ObjectType, Platform*> m_BodyPair;
};
