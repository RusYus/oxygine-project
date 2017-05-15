#pragma once

#include "Constants.h"
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"

namespace Service
{

struct Utils
{
    static b2Vec2 Convert(const oxygine::Vector2& aPos)
    {
        return b2Vec2(aPos.x / Constants::SCALE, aPos.y / Constants::SCALE);
    }

    static oxygine::Vector2 Convert(const b2Vec2& aPos)
    {
        return oxygine::Vector2(aPos.x * Constants::SCALE, aPos.y * Constants::SCALE);
    }
};

}
