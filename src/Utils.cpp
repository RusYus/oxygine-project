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

Velocity::Coordinate::Coordinate(int a_Value)
    : m_Value(a_Value)
{}

Velocity::Coordinate::Type Velocity::Coordinate::operator + (const Coordinate& a_Value)
{
    return m_Value + a_Value.m_Value;
}

Velocity::Coordinate::Type Velocity::Coordinate::operator - (const Coordinate& a_Value)
{
    return m_Value - a_Value.m_Value;
}

void Velocity::Coordinate::operator += (const Coordinate& a_Value)
{
    m_Value += a_Value.m_Value;
}

void Velocity::Coordinate::operator -= (const Coordinate& a_Value)
{
    m_Value -= a_Value.m_Value;
}

bool Velocity::Coordinate::operator < (const Coordinate& a_Value)
{
    return m_Value < a_Value.m_Value;
}

bool Velocity::Coordinate::operator > (const Coordinate& a_Value)
{
    return m_Value > a_Value.m_Value;
}

bool Velocity::Coordinate::operator <= (const Coordinate& a_Value)
{
    return m_Value <= a_Value.m_Value;
}

bool Velocity::Coordinate::operator >= (const Coordinate& a_Value)
{
    return m_Value >= a_Value.m_Value;
}

bool Velocity::Coordinate::operator == (const Coordinate& a_Value)
{
    return m_Value == a_Value.m_Value;
}

bool Velocity::Coordinate::operator != (const Coordinate& a_Value)
{
    return !operator == (a_Value);
}

Velocity::Coordinate::Type Velocity::Coordinate::operator * (const Coordinate& a_Value)
{
    return m_Value * a_Value.m_Value;
}

void Velocity::Coordinate::operator *= (const Coordinate& a_Value)
{
    m_Value *= a_Value.m_Value;
}

template <typename T>
Velocity::Coordinate::Type Velocity::Coordinate::operator + (T a_Value)
{
    return m_Value + a_Value;
}

template <typename T>
Velocity::Coordinate::Type Velocity::Coordinate::operator - (T a_Value)
{
    return m_Value - a_Value;
}

template <typename T>
void Velocity::Coordinate::operator += (T a_Value)
{
    m_Value += a_Value;
}

template <typename T>
void Velocity::Coordinate::operator -= (T a_Value)
{
    m_Value -= a_Value;
}

//template <typename T>
//bool Velocity::Coordinate::operator < (T a_Value)
//{
//    return m_Value < a_Value;
//}

template <typename T>
bool Velocity::Coordinate::operator > (T a_Value)
{
    return m_Value > a_Value;
}

template <typename T>
bool Velocity::Coordinate::operator <= (T a_Value)
{
    return m_Value <= a_Value;
}

template <typename T>
bool Velocity::Coordinate::operator >= (T a_Value)
{
    return m_Value >= a_Value;
}

template <typename T>
bool Velocity::Coordinate::operator == (T a_Value)
{
    return m_Value == a_Value;
}

template <typename T>
bool Velocity::Coordinate::operator != (T a_Value)
{
    return !operator == (a_Value);
}

template <typename T>
Velocity::Coordinate::Type Velocity::Coordinate::operator * (T a_Value)
{
    return m_Value * a_Value;
}

template <typename T>
void Velocity::Coordinate::operator *= (T a_Value)
{
    m_Value *= a_Value;
}

oxygine::Vector2 Velocity::ToVector2() const
{
    return oxygine::Vector2(this->x.m_Value, this->y.m_Value);
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

Velocity Velocity::operator + (const oxygine::Vector2& a_Vector)
{
    Velocity temp = Velocity(this->x + a_Vector.x, this->y + a_Vector.y);
    RoundToNDigits(temp);
    return temp;
}

Velocity Velocity::operator + (const Velocity& a_Vector)
{
    Velocity temp = Velocity(this->x + a_Vector.x, this->y + a_Vector.y);
    RoundToNDigits(temp);
    return temp;
}

Velocity Velocity::operator - (const oxygine::Vector2& a_Vector)
{
    Velocity temp = Velocity(this->x - a_Vector.x, this->y - a_Vector.y);
    RoundToNDigits(temp);
    return temp;
}

Velocity Velocity::operator - (const Velocity& a_Vector)
{
    Velocity temp = Velocity(this->x - a_Vector.x, this->y - a_Vector.y);
    RoundToNDigits(temp);
    return temp;
}

template <class R>
Velocity Velocity::operator * (R a_Value) const
{
//    Velocity temp = oxygine::Vector2::operator *(a_Value);
    Velocity temp(this->x * a_Value, this->y * a_Value);
    RoundToNDigits(temp);
    return temp;
}

template <class R>
Velocity Velocity::operator / (R a_Value) const
{
//    Velocity temp = oxygine::Vector2::operator /(a_Value);
    Velocity temp(this->x / a_Value, this->y / a_Value);
    RoundToNDigits(temp);
    return temp;
}

Velocity& Velocity::operator += (const oxygine::Vector2& a_Vector)
{
    this->x += a_Vector.x;
    this->y += a_Vector.y;
    RoundToNDigits(*this);
    return (*this);
}

Velocity& Velocity::operator -= (const oxygine::Vector2& a_Vector)
{
    this->x -= a_Vector.x;
    this->y -= a_Vector.y;
    RoundToNDigits(*this);
    return (*this);
}

template <typename T>
Velocity Velocity::operator *= (T a_Value)
{
//    oxygine::Vector2::operator *=(a_Value);
    this->x *= a_Value;
    this->y *= a_Value;
    RoundToNDigits(*this);
    return (*this);
}

template <typename T>
Velocity Velocity::operator /= (T a_Value)
{
//    oxygine::Vector2::operator /=(a_Value);
    this->x /= a_Value;
    this->y /= a_Value;
    RoundToNDigits(*this);
    return (*this);
}

void Velocity::Normalize()
{
    Coordinate::Type normal = 1.0 / oxygine::scalar::sqrt(this->x * this->x + this->y * this->y);

    this->x *= normal;
    this->y *= normal;
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
