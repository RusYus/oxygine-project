#pragma once

#include <vector>

class CollisionManager
{
public:
    void AddBody(void*);
    void CheckCollisions();

private:
    std::vector<void*> m_Bodies;
};
