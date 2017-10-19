#pragma once

#include "ICarrier.hpp"
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

    PathNode(unsigned short aId, const Service::Vector2L& aPosition)
    {
        Id = aId;
        Position = aPosition;
    }

    TId Id;
    Service::Vector2L Position;
};

static const constexpr int NODE_SLIPPAGE = 1;

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Actor, public virtual ICarrier, public virtual IDebugDrawable
{
public:
    Platform(const oxygine::Rect&, const std::shared_ptr<ICollisionManager>&);
    ~Platform();
    void Update();
    void SetDirection(const Service::Vector2L&) override;
    void SetPosition() override;
    void CheckCollisions() override;

    void doRender(const oxygine::RenderState&);

private:
    inline bool IsAroundNode();

private:
    PointToPointMode m_RunningMode = PointToPointMode::BackToBack;
    typename PathNode::TId m_NextNodeId = 1;
    bool m_IsMovingReverse = false;
    std::unordered_map<PathNode::TId, PathNode> m_Nodes;
};
