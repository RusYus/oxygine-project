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

struct Velocity
{
private:
    struct Coordinate
    {
    public:
        using Type = float;
    public:
        Coordinate(int);
        Type operator + (const Coordinate& a_Value)
        {
            return m_Value + a_Value.m_Value;
        }
        template <typename T>
        Type operator + (T a_Value)
        {
            return m_Value + a_Value;
        }
        Type operator - (const Coordinate& a_Value)
        {
            return m_Value - a_Value.m_Value;
        }
        template <typename T>
        Type operator - (T a_Value)
        {
            return m_Value - a_Value;
        }
        void operator += (const Coordinate& a_Value)
        {
            m_Value += a_Value.m_Value;
        }
        template <typename T>
        void operator +=(T a_Value)
        {
            m_Value += a_Value;
        }
        void operator -= (const Coordinate& a_Value)
        {
            m_Value -= a_Value.m_Value;
        }
        template <typename T>
        void operator -=(T a_Value)
        {
            m_Value -= a_Value;
        }
    public:
        Type m_Value;
    };

public:
    Velocity();
    Velocity(float a_X, float a_Y);
    Velocity(const oxygine::Vector2& a_Vector);
    Velocity operator + (const oxygine::Vector2& a_Vector);
    Velocity operator + (const Velocity& a_Vector);
    Velocity operator - (const oxygine::Vector2& a_Vector);
    Velocity operator - (const Velocity& a_Vector);
    template <class R>
    Velocity operator * (R a_Vector) const;
    template <class R>
    Velocity operator / (R a_Vector) const;
    Velocity& operator += (const oxygine::Vector2& a_Vector);
    Velocity& operator += (const Velocity& a_Vector);
    Velocity& operator -= (const oxygine::Vector2& a_Vector);
    Velocity& operator -= (const Velocity& a_Vector);
    template <typename T>
    Velocity& operator *= (T a_Value);
    template <typename T>
    Velocity& operator /= (T a_Value);

    Coordinate x, y;
};

static const Normal2 ZeroNormal = Normal2{0, 0};
float RoundToNDigits(float, int /*a_N*/= 2);
void RoundToNDigits(oxygine::Vector2&, int /*a_N*/= 2);
void RoundToNDigits(Velocity&, int /*a_N*/= 2);
}
