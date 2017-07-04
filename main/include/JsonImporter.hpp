#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

#include "MapProperty.hpp"

namespace Service
{

class JsonImporter
{
public:
    bool LoadMap(const std::string&, MapProperty&);

private:
    template<typename T>
    bool LoadTilesLayer(const T&, MapProperty&);
    template<typename T>
    bool LoadObjectsLayer(const T&, MapProperty&);

private:
    std::string mOrientation;
    std::string mRenderorder;

    const std::string mORIENTATION = "orthogonal";
    const std::string mRENDERORDER = "left-up";
};

}
