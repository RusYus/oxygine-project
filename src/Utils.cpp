#include "Utils.hpp"

namespace Service
{

IdGenerator::TId IdGenerator::mId = 0;

float RoundToNDigits(float a_Number, int a_N = 2)
{
    if (a_N < 1 || a_N > 4)
    {
        a_N = 2;
    }
    int ratio = 10 * a_N;
    return std::round(a_Number * ratio) / ratio;
}

void RoundToNDigits(oxygine::Vector2& a_Vector, int a_N = 2)
{
    a_Vector.x = RoundToNDigits(a_Vector.x, a_N);
    a_Vector.x = RoundToNDigits(a_Vector.y, a_N);
}

void RoundToNDigits(Velocity& a_Vector, int a_N = 2)
{
    a_Vector.x = RoundToNDigits(a_Vector.x, a_N);
    a_Vector.x = RoundToNDigits(a_Vector.y, a_N);
}

}
