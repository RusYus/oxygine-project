#pragma once

#include <vector>

#include "DemoLevel.hpp"
#include "Player.hpp"

class CollisionManager
{
public:
    void AddBody(void*);
    void AddBodies(Player*, Static*);
    void CheckCollisions();

private:
    bool Intersection(
        const oxygine::Vector2& /*bottomLeftAABB*/, const oxygine::Vector2& /*topRightAABB*/,
        const oxygine::Vector2& /*startRay*/, const oxygine::Vector2& /*endRay*/,
        oxygine::Vector2& /*intersection*/);

private:
    std::vector<void*> m_Bodies;
    Player* m_Player;
    Static* m_Static;
};
