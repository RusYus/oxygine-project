#include "BasisObject.hpp"

namespace Basis
{

BasisObject::BasisObject()
    : m_Position(0, 0)
{
    m_Id = Service::IdGenerator::GetNextId();
}

}
