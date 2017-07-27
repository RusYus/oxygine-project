#include "JsonImporter.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>
#include <iostream>

namespace Service
{

template<typename T>
bool JsonImporter::LoadTilesLayer(const T& aTileLayer, MapProperty& aOutProperty)
{
    // It is boost::optional<ptree&>
    auto data  = aTileLayer.second.get_child_optional("data");
    // No data in layer.
    if (!data)
    {
        return false;
    }

    aOutProperty.mMapHeight = aTileLayer.second.template get<int>("height");
    aOutProperty.mMapWidth = aTileLayer.second.template get<int>("width");

    aOutProperty.mTilesPositions.reserve(aOutProperty.mMapHeight * aOutProperty.mMapWidth);

    for (auto& dataElement : data.get())
    {
        aOutProperty.mTilesPositions.push_back(std::stoi(dataElement.second.data()));
    }

    return true;
}

template<typename T>
bool JsonImporter::LoadObjectsLayer(const T& aObjectsLayer, MapProperty& aOutProperty)
{
    auto objects = aObjectsLayer.second.get_child_optional("objects");
    if(!objects)
    {
        return false;
    }

    for (auto& object : objects.get())
    {
        MapObject newObject;
        newObject.mWidth = object.second.template get<int>("width");
        newObject.mHeight = object.second.template get<int>("height");
        newObject.mX = object.second.template get<int>("x");
        newObject.mY = object.second.template get<int>("y");
        newObject.mId = object.second.template get<int>("id");

        aOutProperty.mObjects.emplace_back(std::move(newObject));
    }

    return true;
}

bool JsonImporter::LoadMap(const std::string& aMapPath, MapProperty& aOutProperty)
{
    boost::property_tree::ptree root;

    try
    {
        boost::property_tree::read_json(aMapPath, root);
    }
    catch(const boost::property_tree::json_parser_error&)
    {
        return false;
    }


    aOutProperty.mTileHeight = root.get<int>("tileheight");
    aOutProperty.mTileWidth = root.get<int>("tilewidth");

    mOrientation = root.get<std::string>("orientation");
    mRenderorder = root.get<std::string>("renderorder");

    // It is boost::optional<ptree&>
    auto layers = root.get_child_optional("layers");
    // No layers in file.
    if (!layers)
    {
        return false;
    }

    for (const auto& layer : layers.get())
    {
        // Suppose that we already read tile layer and object layer.
        if (!aOutProperty.mTilesPositions.empty() && !aOutProperty.mObjects.empty())
        {
            break;
        }

        if (!LoadTilesLayer(layer, aOutProperty))
        {
            LoadObjectsLayer(layer, aOutProperty);
        }
    }

    return (
        mOrientation == mORIENTATION
        && mRenderorder == mRENDERORDER
        && aOutProperty.IsValidTilesLayer()
        && aOutProperty.IsValidObjectsLayer());
}

}
