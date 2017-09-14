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

// TODO : Move to own file

struct Velocity
{
private:
    struct Coordinate
    {
    public:
        using Type = float;
    public:
        Coordinate(int);
        Type operator + (const Coordinate& a_Value);
        Type operator - (const Coordinate& a_Value);
        void operator += (const Coordinate& a_Value);
        void operator -= (const Coordinate& a_Value);
        bool operator < (const Coordinate& a_Value);
        bool operator > (const Coordinate& a_Value);
        bool operator <= (const Coordinate& a_Value);
        bool operator >= (const Coordinate& a_Value);
        bool operator == (const Coordinate& a_Value);
        bool operator != (const Coordinate& a_Value);
        Type operator * (const Coordinate& a_Value);
        void operator *= (const Coordinate& a_Value);
        template <typename T>
        Type operator + (T a_Value);
        template <typename T>
        Type operator - (T a_Value);
        template <typename T>
        void operator += (T a_Value);
        template <typename T>
        void operator -= (T a_Value);
        template <typename T>
        bool operator < (T a_Value);
        template <typename T>
        bool operator > (T a_Value);
        template <typename T>
        bool operator <= (T a_Value);
        template <typename T>
        bool operator >= (T a_Value);
        template <typename T>
        bool operator == (T a_Value);
        template <typename T>
        bool operator != (T a_Value);
        template <typename T>
        Type operator * (T a_Value);
        template <typename T>
        void operator *= (T a_Value);

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
    Velocity operator * (R a_Value) const;
    template <class R>
    Velocity operator / (R a_Value) const;
    Velocity& operator += (const oxygine::Vector2& a_Vector);
    Velocity& operator += (const Velocity& a_Vector);
    Velocity& operator -= (const oxygine::Vector2& a_Vector);
    Velocity& operator -= (const Velocity& a_Vector);
    template <typename T>
    Velocity operator *= (T a_Value);
    template <typename T>
    Velocity operator /= (T a_Value);

    void Normalize();

public:
    oxygine::Vector2 ToVector2() const;
    Coordinate x, y;
};

template<typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T>
operator + (T a_Value, const Velocity::Coordinate& a_Coord)
{
    return a_Value + a_Coord.m_Value;
}

template<typename T>
std::enable_if_t<std::is_base_of<oxygine::Vector2, T>::value, T>
operator + (T a_Value, const Velocity& a_Vel)
{
    return T(a_Value.x + a_Vel.x, a_Value.y + a_Vel.y);
}


static const Normal2 ZeroNormal = Normal2{0, 0};
float RoundToNDigits(float, int /*a_N*/= 2);
void RoundToNDigits(oxygine::Vector2&, int /*a_N*/= 2);
void RoundToNDigits(Velocity&, int /*a_N*/= 2);
}
