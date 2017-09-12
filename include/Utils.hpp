#pragma once

#include "oxygine-framework.h"
#include "Constants.hpp"

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
    Normal2()
        : oxygine::Vector2()
    {}

    Normal2(float x, float y)
        : oxygine::Vector2(x, y)
    {}

    void operator =(const oxygine::Vector2& a_Vector)
    {
        x = a_Vector.x;
        y = a_Vector.y;
    }

    void operator += (const oxygine::Vector2& a_Vector)
    {
        x += a_Vector.x;
        y += a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator += (const Normal2& a_Vector)
    {
        x += a_Vector.x;
        y += a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const oxygine::Vector2& a_Vector)
    {
        x -= a_Vector.x;
        y -= a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    void operator -= (const Normal2& a_Vector)
    {
        x -= a_Vector.x;
        y -= a_Vector.y;
        FixCoordinatesIfExceeds();
    }

    bool operator == (const Normal2& a_Vector)
    {
        return x == a_Vector.x && y == a_Vector.y;
    }

    bool operator != (const Normal2& a_Vector)
    {
        return !this->operator ==(a_Vector);
    }

    inline void FixCoordinatesIfExceeds()
    {
        if (x < -1)
        {
            x = -1;
        }

        if (x > 1)
        {
            x = 1;
        }

        if (y < -1)
        {
            y = -1;
        }

        if (y > 1)
        {
            y = 1;
        }
    }
};

struct Velocity : oxygine::Vector2
{
public:
    Velocity()
        : oxygine::Vector2()
    {}

    Velocity(float a_X, float a_Y)
        : oxygine::Vector2(a_X, a_Y)
    {}

    Velocity(const oxygine::Vector2& a_Vector)
        : oxygine::Vector2(a_Vector.x, a_Vector.y)
    {}

    Velocity operator + (const VectorT2& a_Vector) const
    {
        Velocity temp = oxygine::Vector2::operator +(a_Vector);
        // TODO : rounding
        return temp;
    }

    Velocity operator - (const VectorT2& a_Vector) const
    {
        Velocity temp = oxygine::Vector2::operator -(a_Vector);
        // TODO : rounding
        return temp;
    }

    template <class R>
    Velocity operator * (R a_Vector) const
    {
        Velocity temp = oxygine::Vector2::operator *(a_Vector);
        // TODO : rounding
        return temp;
    }

    template <class R>
    Velocity operator / (R a_Vector) const
    {
        Velocity temp = oxygine::Vector2::operator /(a_Vector);
        // TODO : rounding
        return temp;
    }

    Velocity& operator += (const VectorT2& a_Vector)
    {
        oxygine::Vector2::operator +=(a_Vector);
        // TODO : rounding
        return (*this);
    }

    Velocity& operator -= (const VectorT2& a_Vector)
    {
        oxygine::Vector2::operator -=(a_Vector);
        // TODO : rounding
        return (*this);
    }

    template <typename T>
    Velocity& operator *= (T a_Value)
    {
        oxygine::Vector2::operator *=(a_Value);
        // TODO : rounding
        return (*this);
    }

    template <typename T>
    Velocity& operator /= (T a_Value)
    {
        oxygine::Vector2::operator /=(a_Value);
        // TODO : rounding
        return (*this);
    }
};

static const Normal2 ZeroNormal = Normal2{0, 0};

float RoundToNDigits(float, int);

void RoundToNDigits(oxygine::Vector2&, int);
void RoundToNDigits(oxygine::Point&, int);

}
