#pragma once

#include "oxygine-framework.h"

#include "Constants.hpp"
#include "Velocity.hpp"

namespace Service
{

class IdGenerator
{
public:
    using TId = int;
    static inline TId GetNextId()
    {
        return ++mId;
    }

private:
    static TId mId;
};

struct Normal2 : oxygine::Vector2
{
public:
    Normal2();
    Normal2(float x, float y);
    void operator =(const oxygine::Vector2& a_Vector);
    void operator += (const oxygine::Vector2& a_Vector);
    void operator += (const Normal2& a_Vector);
    void operator -= (const oxygine::Vector2& a_Vector);
    void operator -= (const Normal2& a_Vector);
    bool operator == (const Normal2& a_Vector);
    bool operator != (const Normal2& a_Vector);
    inline void FixCoordinatesIfExceeds();
};

static const Normal2 ZeroNormal = Normal2{0, 0};
void RoundToNDigits(Model::Velocity&, int /*a_N*/= 2);
void RoundToNDigits(oxygine::Vector2&, int /*a_N*/= 2);
float RoundToNDigits(float, int /*a_N*/= 2);
}
