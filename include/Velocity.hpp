#pragma once

#include <type_traits>

#include "math/Vector2.h"

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
        Coordinate(int a_Value);
        Type operator + (const Coordinate& a_Value);
        Type operator - (const Coordinate& a_Value);
        void operator += (const Coordinate& a_Value);
        void operator -= (const Coordinate& a_Value);
        bool operator < (const Coordinate& a_Value) const;
        bool operator > (const Coordinate& a_Value) const;
        bool operator <= (const Coordinate& a_Value) const;
        bool operator >= (const Coordinate& a_Value) const;
        bool operator == (const Coordinate& a_Value) const;
        bool operator != (const Coordinate& a_Value) const;
        Type operator * (const Coordinate& a_Value);
        void operator *= (const Coordinate& a_Value);

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
        bool operator < (T a_Value) const
        {
            return m_Value < a_Value;
        }

        template <typename T>
        bool operator > (T a_Value) const
        {
            return m_Value > a_Value;
        }

        template <typename T>
        bool operator <= (T a_Value) const
        {
            return m_Value <= a_Value;
        }

        template <typename T>
        bool operator >= (T a_Value) const
        {
            return m_Value >= a_Value;
        }

        template <typename T>
        bool operator == (T a_Value) const
        {
            return m_Value == a_Value;
        }

        template <typename T>
        bool operator != (T a_Value) const
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
    Velocity();
    Velocity(float a_X, float a_Y);
    Velocity(const oxygine::Vector2& a_Vector);
    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator + (const T& a_Vector)
    {
        Velocity temp = Velocity(this->x + a_Vector.x, this->y + a_Vector.y);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator - (const T& a_Vector)
    {
        Velocity temp = Velocity(this->x - a_Vector.x, this->y - a_Vector.y);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator * (const T& a_Vector)
    {
        Velocity temp = Velocity(this->x * a_Vector.x, this->y * a_Vector.y);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator / (const T& a_Vector)
    {
        Velocity temp = Velocity(this->x / a_Vector.x, this->y / a_Vector.y);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity&>
    operator += (const T& a_Vector)
    {
        this->x += a_Vector.x;
        this->y += a_Vector.y;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity&>
    operator -= (const T& a_Vector)
    {
        this->x -= a_Vector.x;
        this->y -= a_Vector.y;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity&>
    operator *= (const T& a_Vector)
    {
        this->x *= a_Vector.x;
        this->y *= a_Vector.y;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, Velocity&>
    operator /= (const T& a_Vector)
    {
        this->x /= a_Vector.x;
        this->y /= a_Vector.y;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, bool>
    operator == (const T& a_Vector) const
    {
        return this->x == a_Vector.x && this->y == a_Vector.y;
    }

    template <typename T>
    typename std::enable_if_t<
        std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        || std::is_same<std::decay_t<T>, Velocity>::value, bool>
    operator != (const T& a_Vector) const
    {
        return !operator == (a_Vector);
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator + (T a_Value)
    {
    //    Velocity temp = oxygine::Vector2::operator *(a_Value);
        Velocity temp(this->x + a_Value, this->y + a_Value);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator - (T a_Value)
    {
    //    Velocity temp = oxygine::Vector2::operator *(a_Value);
        Velocity temp(this->x - a_Value, this->y - a_Value);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator * (T a_Value)
    {
    //    Velocity temp = oxygine::Vector2::operator *(a_Value);
        Velocity temp(this->x * a_Value, this->y * a_Value);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator / (T a_Value)
    {
    //    Velocity temp = oxygine::Vector2::operator /(a_Value);
        Velocity temp(this->x / a_Value, this->y / a_Value);
        RoundToNDigits(temp);
        return temp;
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator += (T a_Value)
    {
        this->x += a_Value;
        this->y += a_Value;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator -= (T a_Value)
    {
        this->x -= a_Value;
        this->y -= a_Value;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator *= (T a_Value)
    {
        this->x *= a_Value;
        this->y *= a_Value;
        RoundToNDigits(*this);
        return (*this);
    }

    template <typename T>
    typename std::enable_if_t<
        !std::is_same<std::decay_t<T>, oxygine::Vector2>::value
        && !std::is_same<std::decay_t<T>, Velocity>::value, Velocity>
    operator /= (T a_Value)
    {
        this->x /= a_Value;
        this->y /= a_Value;
        RoundToNDigits(*this);
        return (*this);
    }
public:
    void Normalize();
    oxygine::Vector2 ToVector2() const;
public:
    Coordinate x, y;
private:
    static constexpr const unsigned int m_MAX_ROUNDING_DIGITS_COUNT = 4;
    std::array<unsigned int, m_MAX_ROUNDING_DIGITS_COUNT> m_RoundingDegrees{100, 1000, 10000};
    float RoundToNDigits(float a_Number, int a_N = 2);
    void RoundToNDigits(oxygine::Vector2& a_Vector, int a_N = 2);
    void RoundToNDigits(Velocity& a_Vector, int a_N = 2);
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
