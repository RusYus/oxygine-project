#include "JsonImporter.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>
#include <iostream>

namespace Service
{

inline bool JsonImporter::IsValidTilesLayer() const
{
    return (_mapHeight > 0 && _mapWidth > 0 && _tileHeight > 0 && _tileWidth > 0
        && _tilePositions.size() == _mapHeight * _mapWidth);
}

inline bool JsonImporter::IsValidObjectsLayer() const
{
    const auto& invalidObject = std::find_if(_objects.cbegin(), _objects.cend(),
        [] (const MapObject& object)
        {
            return ( object._width <= 0 || object._height <= 0
                || object._x < 0 || object._y < 0
                || object._id < 0);
        });

    return (invalidObject == _objects.cend());
}

inline bool JsonImporter::IsValidMap() const
{
    if (_orientation != _ORIENTATION || _renderorder != _RENDERORDER
        || !IsValidTilesLayer() || !IsValidObjectsLayer())
    {
        return false;
    }
    else
    {
        return true;
    }
}

template<typename T>
bool JsonImporter::LoadTilesLayer(const T& aTileLayer)
{
    // It is boost::optional<ptree&>
    auto data  = aTileLayer.second.get_child_optional("data");
    // No data in layer.
    if (!data)
    {
        return false;
    }

    _mapHeight = aTileLayer.second.template get<int>("height");
    _mapWidth = aTileLayer.second.template get<int>("width");

    _tilePositions.reserve(_mapHeight * _mapWidth);

    for (auto& dataElement : data.get())
    {
        _tilePositions.push_back(std::stoi(dataElement.second.data()));
    }

    return true;
}

template<typename T>
bool JsonImporter::LoadObjectsLayer(const T& aObjectsLayer)
{
    auto objects = aObjectsLayer.second.get_child_optional("objects");
    if(!objects)
    {
        return false;
    }

    for (auto& object : objects.get())
    {
        MapObject newObject;
        newObject._width = object.second.template get<int>("width");
        newObject._height = object.second.template get<int>("height");
        newObject._x = object.second.template get<int>("x");
        newObject._y = object.second.template get<int>("y");
        newObject._id = object.second.template get<int>("id");

        _objects.emplace_back(std::move(newObject));
    }

    return true;
}

bool JsonImporter::LoadMap(const std::string& aMapPath)
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
    catch (...)
    {
        return false;
    }

    _tileHeight = root.get<int>("tileheight");
    _tileWidth = root.get<int>("tilewidth");

    _orientation = root.get<std::string>("orientation");
    _renderorder = root.get<std::string>("renderorder");

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
        if (!_tilePositions.empty() && !_objects.empty())
        {
            break;
        }

        if (!LoadTilesLayer(layer))
        {
            LoadObjectsLayer(layer);
        }
    }

    return IsValidMap();
}

}
