#pragma once

#include <vector>
#include <iostream>

#include "DemoLevel.hpp"
#include "Player.hpp"

class CollisionManager
{
    using Body = std::pair<Basis::BasisObject*, bool /*isMovable*/>;
public:
    template<typename BodyType>
    void AddBody(BodyType* aBody)
    {
        Body body = std::make_pair(static_cast<Basis::BasisObject*>(aBody), false);
        // Check whether it MovableObject or not (derived from BasisObject?)
        if (std::is_base_of<IMovable, BodyType>::value)
        {
            std::cout << "Base or same type " << std::endl;
            body.second = true;
        }
        else
        {
            std::cout << "Not the same type " << std::endl;
            body.second = false;
        }

        m_Bodies.emplace_back(std::move(body));
    }

    void CheckCollisions();
private:
    bool Intersection(
        const oxygine::Vector2& /*bottomLeftAABB*/, const oxygine::Vector2& /*topRightAABB*/,
        const oxygine::Vector2& /*startRay*/, const oxygine::Vector2& /*endRay*/,
        oxygine::Vector2& /*intersection*/);

private:
    std::vector<Body> m_Bodies;
};
