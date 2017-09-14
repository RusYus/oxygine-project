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
    // TODO : Maybe another overloading to hide m_Value ?
    a_Vector.x = RoundToNDigits(a_Vector.x.m_Value, a_N);
    a_Vector.x = RoundToNDigits(a_Vector.y.m_Value, a_N);
}

}
