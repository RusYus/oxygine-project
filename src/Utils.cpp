#include "Utils.hpp"

namespace Service
{

Normal2::Normal2()
    : oxygine::Vector2()
{}

Normal2::Normal2(float x, float y)
    : oxygine::Vector2(x, y)
{}

void Normal2::operator =(const oxygine::Vector2& a_Vector)
{
    x = a_Vector.x;
    y = a_Vector.y;
}

void Normal2::operator += (const oxygine::Vector2& a_Vector)
{
    x += a_Vector.x;
    y += a_Vector.y;
    FixCoordinatesIfExceeds();
}

void Normal2::operator += (const Normal2& a_Vector)
{
    x += a_Vector.x;
    y += a_Vector.y;
    FixCoordinatesIfExceeds();
}

void Normal2::operator -= (const oxygine::Vector2& a_Vector)
{
    x -= a_Vector.x;
    y -= a_Vector.y;
    FixCoordinatesIfExceeds();
}

void Normal2::operator -= (const Normal2& a_Vector)
{
    x -= a_Vector.x;
    y -= a_Vector.y;
    FixCoordinatesIfExceeds();
}

bool Normal2::operator == (const Normal2& a_Vector)
{
    return x == a_Vector.x && y == a_Vector.y;
}

bool Normal2::operator != (const Normal2& a_Vector)
{
    return !this->operator ==(a_Vector);
}

inline void Normal2::FixCoordinatesIfExceeds()
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

Velocity::Velocity()
    : oxygine::Vector2()
{}

Velocity::Velocity(float a_X, float a_Y)
    : oxygine::Vector2(a_X, a_Y)
{}

Velocity::Velocity(const oxygine::Vector2& a_Vector)
    : oxygine::Vector2(a_Vector.x, a_Vector.y)
{}

Velocity Velocity::operator + (const VectorT2& a_Vector) const
{
    Velocity temp = oxygine::Vector2::operator +(a_Vector);
    RoundToNDigits(temp);
    return temp;
}

Velocity Velocity::operator - (const VectorT2& a_Vector) const
{
    Velocity temp = oxygine::Vector2::operator -(a_Vector);
    RoundToNDigits(temp);
    return temp;
}

template <class R>
Velocity Velocity::operator * (R a_Vector) const
{
    Velocity temp = oxygine::Vector2::operator *(a_Vector);
    RoundToNDigits(temp);
    return temp;
}

template <class R>
Velocity Velocity::operator / (R a_Vector) const
{
    Velocity temp = oxygine::Vector2::operator /(a_Vector);
    RoundToNDigits(temp);
    return temp;
}

Velocity& Velocity::operator += (const VectorT2& a_Vector)
{
    oxygine::Vector2::operator +=(a_Vector);
    RoundToNDigits(*this);
    return (*this);
}

Velocity& Velocity::operator -= (const VectorT2& a_Vector)
{
    oxygine::Vector2::operator -=(a_Vector);
    RoundToNDigits(*this);
    return (*this);
}

template <typename T>
Velocity& Velocity::operator *= (T a_Value)
{
    oxygine::Vector2::operator *=(a_Value);
    RoundToNDigits(*this);
    return (*this);
}

template <typename T>
Velocity& Velocity::operator /= (T a_Value)
{
    oxygine::Vector2::operator /=(a_Value);
    RoundToNDigits(*this);
    return (*this);
}

IdGenerator::TId IdGenerator::mId = 0;

float RoundToNDigits(float a_Number, int a_N)
{
    if (a_N < 1 || a_N > 4)
    {
        a_N = 2;
    }
    int ratio = 10 * a_N;
    return std::round(a_Number * ratio) / ratio;
}

void RoundToNDigits(oxygine::Vector2& a_Vector, int a_N)
{
    a_Vector.x = RoundToNDigits(a_Vector.x, a_N);
    a_Vector.x = RoundToNDigits(a_Vector.y, a_N);
}

void RoundToNDigits(Velocity& a_Vector, int a_N)
{
    a_Vector.x = RoundToNDigits(a_Vector.x, a_N);
    a_Vector.x = RoundToNDigits(a_Vector.y, a_N);
}

}
