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

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Box9Sprite, public Basis::BasisObject
{
public:
    Platform(b2World*, const oxygine::RectF&);
    void Move();

private:
    PointToPointMode m_RunningMode = PointToPointMode::BackToBack;
    typename PathNode::TId m_CurrentPointId = 0;
    typename PathNode::TId m_NextPointId = 1;
    bool m_IsMovingReverse = false;
    b2Vec2 m_Direction;
    std::unordered_map<PathNode::TId, PathNode> m_Points;
    std::pair<Service::ObjectType, Platform*> m_BodyPair;
    b2Body* m_Body;
};
