#pragma once

#include "oxygine-framework.h"

#include "Constants.hpp"

namespace Service
{

inline bool AreEqual(float a_Ls, float a_Rs)
{
    if (std::fabs(a_Ls) < Constants::EPSILON && std::fabs(a_Rs) < Constants::EPSILON)
    {
        return true;
    }

    return std::fabs(a_Ls - a_Rs) < Constants::EPSILON;
}

using TCoordinate = long long;
typedef oxygine::VectorT2<TCoordinate> Vector2L;

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

}
