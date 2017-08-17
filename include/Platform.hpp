#pragma once

#include "IMovable.hpp"
#include "IDebugDrawable.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"

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

static const constexpr float32 NODE_SLIPPAGE = 1;

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Actor, public virtual IMovable, public virtual IDebugDrawable
{
public:
    Platform(const oxygine::RectF&);
    void Move();
    void SetDirection(const oxygine::Vector2&, bool /*a_SetExact*/ = false) override;
    void SetPosition() override;
    void ResetCollisionNormal(const Collision::CollisionInfo&) override;

    void doRender(const oxygine::RenderState&);

private:
    inline bool IsAroundNode();

private:
    PointToPointMode m_RunningMode = PointToPointMode::BackToBack;
    typename PathNode::TId m_NextNodeId = 1;
    bool m_IsMovingReverse = false;
    oxygine::Vector2 m_DirectionUntilStop;
    std::unordered_map<PathNode::TId, PathNode> m_Nodes;
    std::pair<Service::ObjectType, Platform*> m_BodyPair;
};
