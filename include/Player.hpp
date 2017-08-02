#pragma once

#include "RenderState.h"
#include "core/gl/ShaderProgramGL.h"


#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

constexpr const int PLAYER_MAX_SPEED =4;
constexpr const int PLAYER_JUMP_SPEED = 5;
constexpr const float GRAVITY = 50;
constexpr const int RAYCAST_INTERVAL = 50;

struct Ray
{
    Ray(const oxygine::Vector2& aOriginal, const oxygine::Vector2& aDestination)
        : Original(aOriginal)
        , Destination(aDestination)
        , IsHitInLastStep(false)
        , IsHitInCurrentStep(false)
    {}

    oxygine::Vector2 Original;
    oxygine::Vector2 Destination;
    bool IsHitInLastStep;
    bool IsHitInCurrentStep;
};

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Object, public Basis::BasisObject
{
public:
    Player();
    ~Player();
    void Init(spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void ProcessMoveEvent(oxygine::Event*);
    void Jump(oxygine::Event*);
     void SetY(float);
     void SetX(float);
     float GetX() const;
     float GetY() const;
     float GetWidth() const;
     float GetHeight() const;
     oxygine::Vector2 GetRayOriginal() const;
     std::vector<Ray>& GetRays();
     oxygine::Vector2 GetRayDestination() const;
     void SetPosition();
     oxygine::Vector2 GetPosition() const;
     oxygine::Vector2 GetDirection() const;
     Service::Normal2 GetCollisionNormal() const;
    void SetCollisionNormal(const oxygine::Vector2);
    void SetZeroCollisionNormal();

    void doRender(const oxygine::RenderState& rs);

private:
    void Move(bool /*aIsMovingRight*/);
    inline void Stop();
    void ProcessKeyboard();

private:
    spEventProxy mEventProxy;
    oxygine::spActor mView;
    oxygine::spSprite mBox;
    oxygine::Vector2 mDirection;
    Service::Normal2 mCollisionNormal;
    std::vector<Ray> mRays;
    bool mIsJumping;
    bool mIsButtonMoving;
    const int mMaxSpeed = PLAYER_MAX_SPEED;
    const int mJumpSpeed = PLAYER_JUMP_SPEED;

    std::pair<Service::ObjectType, Player*> mBodyPair;

    static const int MAX_VERTICES = 64;
    static const int CIRCLE_SEGMENTS = 16;

    void createCircleVertices(const oxygine::Vector2& aCenter, float aRadius);
    oxygine::Vector2 mVertices[MAX_VERTICES];
    void drawPrimitives(bool drawLines, int aCount, const oxygine::Color& aColor);

    typename oxygine::ShaderProgramGL* _program;
};
