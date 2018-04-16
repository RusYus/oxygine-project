#include "Utils.hpp"

namespace Service
{

Normal2::Normal2()
    : oxygine::Point()
{}

Normal2::Normal2(int a_X, int a_Y)
    : oxygine::Point(a_X, a_Y)
{}

void Normal2::operator =(const oxygine::Point& a_Vector)
{
    x = a_Vector.x;
    y = a_Vector.y;
}

void Normal2::operator += (const oxygine::Point& a_Vector)
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

void Normal2::operator -= (const oxygine::Point& a_Vector)
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

}

namespace oxygine
{
    std::ostream& operator << (std::ostream& a_Os, const Service::Vector2L& a_Vector)
    {
        a_Os << a_Vector.x << ":"  << a_Vector.y << " ";
        return a_Os;
    }
}
