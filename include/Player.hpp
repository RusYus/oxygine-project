#pragma once

#include "BasisEventProxy.hpp"
#include "CollisionInfo.hpp"
#include "IMovable.hpp"
#include "IDebugDrawable.hpp"
#include "Utils.hpp"

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Actor, public virtual IMovable, public virtual IDebugDrawable
{
public:
    Player(const std::shared_ptr<ICollisionManager>&);
    ~Player();
    void SetPosition() override;

    void Init(spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void ProcessMoveEvent(oxygine::Event*);
    void ProcessJumpEvent(oxygine::Event*);
    void doRender(const oxygine::RenderState&);
    void AddDirection(const Service::Vector2L&) override;
    bool IsJumping() const;

private:
    void Move(bool /*aIsMovingRight*/);
    inline void Stop();
    void ProcessKeyboard();
    void Jump();

private:
    spEventProxy m_EventProxy;
    bool m_IsJumping;
    bool m_IsButtonMoving;
    const int m_MaxSpeed = Service::Constants::PLAYER_MAX_SPEED;
    const int m_JumpSpeed = Service::Constants::PLAYER_JUMP_SPEED;
};
