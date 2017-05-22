#include "JsonImporter.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Service
{

inline int JsonImporter::GetMapHeight() const
{
    return _mapHeight;
}

inline int JsonImporter::GetMapWidth() const
{
    return _mapWidth;
}

inline int JsonImporter::GetTileHeight() const
{
    return _tileHeight;
}

inline int JsonImporter::GetTileWidth() const
{
    return _tileWidth;
}

inline bool JsonImporter::IsValidMap() const
{
    if (_orientation != _ORIENTATION || _renderorder != _RENDERORDER
        || _mapHeight < 0 || _mapWidth < 0 || _tileHeight < 0 || _tileWidth < 0
        || _tilePositions.size() < _mapHeight * _mapWidth)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool JsonImporter::LoadMap()
{
    boost::property_tree::ptree root;

    try
    {
        boost::property_tree::read_json("map.json", root);
    }
    catch(const boost::property_tree::json_parser_error&)
    {
        return false;
    }

    _tileHeight = root.get<int>("tileheight");
    _tileWidth = root.get<int>("tilewidth");

    _orientation = root.get<std::string>("orientation");
    _renderorder = root.get<std::string>("renderorder");

    for (const auto& layer : root.get_child("layers"))
    {
        _mapHeight = layer.second.get<int>("height");
        _mapWidth = layer.second.get<int>("width");

        _tilePositions.reserve(_mapHeight * _mapWidth);
        for (auto& dataElement : layer.second.get_child("data"))
        {
            _tilePositions.push_back(std::stoi(dataElement.second.data()));
        }

        // Don't need more than 1 layer.
        break;
    }

    return IsValidMap();
}

}
