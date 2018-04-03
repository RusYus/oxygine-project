#pragma once

#include <iostream>

namespace Common
{

struct CarrierInfo
{
    CarrierInfo()
        : Id(Service::IdGenerator::UnknownId)
        , Direction(0, 0)
    {
        std::cout << "New CarrierInfo!" << std::endl;
    }

    Basis::BasisObject::TId Id;
    Service::Vector2L Direction;
};

}
