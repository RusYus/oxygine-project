#pragma once

#include "math/Vector2.h"

#include "Utils.hpp"

namespace Model
{

struct Velocity
{
private:
    struct Coordinate
    {
    public:
        using Type = float;
    public:
        Coordinate(int a_Value)
            : m_Value(a_Value)
        {}

        Type operator + (const Coordinate& a_Value)
        {
            return m_Value + a_Value.m_Value;
        }

        Type operator - (const Coordinate& a_Value)
        {
            return m_Value - a_Value.m_Value;
        }

        void operator += (const Coordinate& a_Value)
        {
            m_Value += a_Value.m_Value;
        }

        void operator -= (const Coordinate& a_Value)
        {
            m_Value -= a_Value.m_Value;
        }

        bool operator < (const Coordinate& a_Value)
        {
            return m_Value < a_Value.m_Value;
        }

        bool operator > (const Coordinate& a_Value)
        {
            return m_Value > a_Value.m_Value;
        }

        bool operator <= (const Coordinate& a_Value)
        {
            return m_Value <= a_Value.m_Value;
        }

        bool operator >= (const Coordinate& a_Value)
        {
            return m_Value >= a_Value.m_Value;
        }

        bool operator == (const Coordinate& a_Value)
        {
            return m_Value == a_Value.m_Value;
        }

        bool operator != (const Coordinate& a_Value)
        {
            return !operator == (a_Value);
        }

        Type operator * (const Coordinate& a_Value)
        {
            return m_Value * a_Value.m_Value;
        }

        void operator *= (const Coordinate& a_Value)
        {
            m_Value *= a_Value.m_Value;
        }

        template <typename T>
        Type operator + (T a_Value)
        {
            return m_Value + a_Value;
        }

        template <typename T>
        Type operator - (T a_Value)
        {
            return m_Value - a_Value;
        }

        template <typename T>
        void operator += (T a_Value)
        {
            m_Value += a_Value;
        }

        template <typename T>
        void operator -= (T a_Value)
        {
            m_Value -= a_Value;
        }

        template <typename T>
        bool operator < (T a_Value)
        {
            return m_Value < a_Value;
        }

        template <typename T>
        bool operator > (T a_Value)
        {
            return m_Value > a_Value;
        }

        template <typename T>
        bool operator <= (T a_Value)
        {
            return m_Value <= a_Value;
        }

        template <typename T>
        bool operator >= (T a_Value)
        {
            return m_Value >= a_Value;
        }

        template <typename T>
        bool operator == (T a_Value)
        {
            return m_Value == a_Value;
        }

        template <typename T>
        bool operator != (T a_Value)
        {
            return !operator == (a_Value);
        }

        template <typename T>
        Type operator * (T a_Value)
        {
            return m_Value * a_Value;
        }

        template <typename T>
        void operator *= (T a_Value)
        {
            m_Value *= a_Value;
        }
    public:
        Type m_Value;
    };

public:
    Velocity()
        : x(0)
        , y(0)
    {}

    Velocity(float a_X, float a_Y)
        : x(a_X)
        , y(a_Y)
    {}

    Velocity(const oxygine::Vector2& a_Vector)
        : x(a_Vector.x)
        , y(a_Vector.y)
    {}

    Velocity operator + (const oxygine::Vector2& a_Vector)
    {
        Velocity temp = Velocity(this->x + a_Vector.x, this->y + a_Vector.y);
        Service::RoundToNDigits(temp);
        return temp;
    }

    Velocity operator + (const Velocity& a_Vector)
    {
        Velocity temp = Velocity(this->x + a_Vector.x, this->y + a_Vector.y);
        Service::RoundToNDigits(temp);
        return temp;
    }

    Velocity operator - (const oxygine::Vector2& a_Vector)
    {
        Velocity temp = Velocity(this->x - a_Vector.x, this->y - a_Vector.y);
        Service::RoundToNDigits(temp);
        return temp;
    }

    Velocity operator - (const Velocity& a_Vector)
    {
        Velocity temp = Velocity(this->x - a_Vector.x, this->y - a_Vector.y);
        Service::RoundToNDigits(temp);
        return temp;
    }

    template <class R>
    Velocity operator * (R a_Value) const
    {
    //    Velocity temp = oxygine::Vector2::operator *(a_Value);
        Velocity temp(this->x * a_Value, this->y * a_Value);
        Service::RoundToNDigits(temp);
        return temp;
    }

    template <class R>
    Velocity operator / (R a_Value) const
    {
    //    Velocity temp = oxygine::Vector2::operator /(a_Value);
        Velocity temp(this->x / a_Value, this->y / a_Value);
        Service::RoundToNDigits(temp);
        return temp;
    }

    Velocity& operator += (const oxygine::Vector2& a_Vector)
    {
        this->x += a_Vector.x;
        this->y += a_Vector.y;
        Service::RoundToNDigits(*this);
        return (*this);
    }

    Velocity& operator -= (const oxygine::Vector2& a_Vector)
    {
        this->x -= a_Vector.x;
        this->y -= a_Vector.y;
        Service::RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    Velocity operator *= (T a_Value)
    {
    //    oxygine::Vector2::operator *=(a_Value);
        this->x *= a_Value;
        this->y *= a_Value;
        Service::RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    Velocity operator /= (T a_Value)
    {
    //    oxygine::Vector2::operator /=(a_Value);
        this->x /= a_Value;
        this->y /= a_Value;
        Service::RoundToNDigits(*this);
        return (*this);
    }
public:
    void Normalize()
    {
        Coordinate::Type normal = 1.0 / oxygine::scalar::sqrt(this->x * this->x + this->y * this->y);

        this->x *= normal;
        this->y *= normal;
    }

public:
    oxygine::Vector2 ToVector2() const
    {
        return oxygine::Vector2(this->x.m_Value, this->y.m_Value);
    }
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

}
