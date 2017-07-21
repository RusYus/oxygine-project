#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"

DECLARE_SMART(Platform, spPlatform);
class Platform: public oxygine::Box9Sprite, public Basis::BasisObject
{
public:
    Platform(b2World*, const oxygine::RectF&);
    void Move(b2Vec2);
    b2Body* m_Body;

private:
    std::pair<Service::ObjectType, Platform*> m_BodyPair;
};
