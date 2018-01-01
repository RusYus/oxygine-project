#pragma once

namespace Common
{

struct CarrierInfo
{
    CarrierInfo()
        : Id(Service::IdGenerator::UnknownId)
        , Direction(0, 0)
    {}

    Basis::BasisObject::TId Id;
    Service::Vector2L Direction;
};

}
