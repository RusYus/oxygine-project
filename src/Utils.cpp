#include "Utils.hpp"

namespace Service
{

IdGenerator::TId IdGenerator::mId = 0;

float RoundToOneDigit(float a_Number)
{
    return std::round(a_Number * 10) / 10;
}

void RoundToOneDigit(oxygine::Vector2& a_Vector)
{
    a_Vector.x = RoundToOneDigit(a_Vector.x);
    a_Vector.x = RoundToOneDigit(a_Vector.y);
}

}
