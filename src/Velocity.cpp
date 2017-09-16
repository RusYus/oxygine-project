#include "Velocity.hpp"

namespace Model
{

Velocity::Coordinate::Coordinate(int a_Value)
    : m_Value(a_Value)
{}

Velocity::Coordinate::Type Velocity::Coordinate::operator + (const Velocity::Coordinate& a_Value)
{
    return m_Value + a_Value.m_Value;
}

Velocity::Coordinate::Type Velocity::Coordinate::operator - (const Velocity::Coordinate& a_Value)
{
    return m_Value - a_Value.m_Value;
}

void Velocity::Coordinate::operator += (const Velocity::Coordinate& a_Value)
{
    m_Value += a_Value.m_Value;
}

void Velocity::Coordinate::operator -= (const Velocity::Coordinate& a_Value)
{
    m_Value -= a_Value.m_Value;
}

bool Velocity::Coordinate::operator < (const Velocity::Coordinate& a_Value) const
{
    return m_Value < a_Value.m_Value;
}

bool Velocity::Coordinate::operator > (const Velocity::Coordinate& a_Value) const
{
    return m_Value > a_Value.m_Value;
}

bool Velocity::Coordinate::operator <= (const Velocity::Coordinate& a_Value) const
{
    return m_Value <= a_Value.m_Value;
}

bool Velocity::Coordinate::operator >= (const Velocity::Coordinate& a_Value) const
{
    return m_Value >= a_Value.m_Value;
}

bool Velocity::Coordinate::operator == (const Velocity::Coordinate& a_Value) const
{
    return m_Value == a_Value.m_Value;
}

bool Velocity::Coordinate::operator != (const Velocity::Coordinate& a_Value) const
{
    return !Velocity::Coordinate::operator == (a_Value);
}

Velocity::Coordinate::Type Velocity::Coordinate::operator * (const Velocity::Coordinate& a_Value)
{
    return m_Value * a_Value.m_Value;
}

void Velocity::Coordinate::operator *= (const Velocity::Coordinate& a_Value)
{
    m_Value *= a_Value.m_Value;
}


Velocity::Velocity()
    : x(0)
    , y(0)
{}

Velocity::Velocity(float a_X, float a_Y)
    : x(a_X)
    , y(a_Y)
{}

Velocity::Velocity(const oxygine::Vector2& a_Vector)
    : x(a_Vector.x)
    , y(a_Vector.y)
{}

void Velocity::Normalize()
{
    Coordinate::Type normal = 1.0 / oxygine::scalar::sqrt(this->x * this->x + this->y * this->y);

    this->x *= normal;
    this->y *= normal;
}

oxygine::Vector2 Velocity::ToVector2() const
{
    return oxygine::Vector2(this->x.m_Value, this->y.m_Value);
}

float Velocity::RoundToNDigits(float a_Number, int a_N)
{
    if (a_N < 2 || a_N > m_MAX_ROUNDING_DIGITS_COUNT)
    {
        a_N = 2;
    }

    return std::round(a_Number * m_RoundingDegrees[a_N - 2]) / m_RoundingDegrees[a_N - 2];
}

void Velocity::RoundToNDigits(oxygine::Vector2& a_Vector, int a_N)
{
    a_Vector.x = RoundToNDigits(a_Vector.x, a_N);
    a_Vector.y = RoundToNDigits(a_Vector.y, a_N);
}

void Velocity::RoundToNDigits(Velocity& a_Vector, int a_N)
{
    // TODO : Maybe another overloading to hide m_Value ?
    a_Vector.x = RoundToNDigits(a_Vector.x.m_Value, a_N);
    a_Vector.y = RoundToNDigits(a_Vector.y.m_Value, a_N);
}

}

