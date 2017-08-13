#pragma once

#include "RenderState.h"
#include "core/gl/ShaderProgramGL.h"


#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "CollisionInfo.hpp"
#include "IMovable.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Actor, public IMovable
{
public:
    Player();
    ~Player();
    void SetPosition() override;

    void Init(spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void ProcessMoveEvent(oxygine::Event*);
    void Jump(oxygine::Event*);

    void SetDebugDraw(bool);
    bool GetDebugDraw() const;

    void doRender(const oxygine::RenderState& rs);

private:
    void Move(bool /*aIsMovingRight*/);
    inline void Stop();
    void ProcessKeyboard();

private:
    spEventProxy mEventProxy;
    bool mIsJumping;
    bool mIsButtonMoving;
    const int mMaxSpeed = Service::Constants::PLAYER_MAX_SPEED;
    const int mJumpSpeed = Service::Constants::PLAYER_JUMP_SPEED;

    std::pair<Service::ObjectType, Player*> mBodyPair;

    static const int MAX_VERTICES = 64;
    static const int CIRCLE_SEGMENTS = 16;

    void createCircleVertices(const oxygine::Vector2& aCenter, int aRadius);
    oxygine::Vector2 mVertices[MAX_VERTICES];
    void drawPrimitives(int aCount, const oxygine::Color& aColor);

    typename oxygine::ShaderProgramGL* _program;
};
