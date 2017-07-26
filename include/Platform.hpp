#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"

enum class PointToPointMode
{
    Cycle,
    BackToBack,
};

struct PathNode
{
    using TId = unsigned short;

    PathNode(unsigned short aId, int aX, int aY)
    {
        Id = aId;
        X = aX;
        Y = aY;
    }

    PathNode(unsigned short aId, const b2Vec2& aPosition)
    {
        Id = aId;
        Position = aPosition;
    }

    TId Id;
    b2Vec2 Position;
    int X;
    int Y;
};

static const constexpr float32 NODE_SLIPPAGE = 0.0001;

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Box9Sprite, public Basis::BasisObject
{
public:
    Platform(b2World*, const oxygine::RectF&);
    void Move();

private:
    bool IsAroundNode();

private:
    PointToPointMode m_RunningMode = PointToPointMode::BackToBack;
    typename PathNode::TId m_NextNodeId = 1;
    bool m_IsMovingReverse = false;
    b2Vec2 m_Direction;
    const int m_Speed = 4;
    std::unordered_map<PathNode::TId, PathNode> m_Nodes;
    std::pair<Service::ObjectType, Platform*> m_BodyPair;
    b2Body* m_Body;
};