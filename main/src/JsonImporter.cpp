#include "JsonImporter.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>
#include <iostream>

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
        || _mapHeight <= 0 || _mapWidth <= 0 || _tileHeight <= 0 || _tileWidth <= 0
        || _tilePositions.size() != _mapHeight * _mapWidth)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool JsonImporter::LoadMap(const std::string& aMapPath)
{
    boost::property_tree::ptree root;

    try
    {
        boost::property_tree::read_json(aMapPath, root);
    }
    catch(const boost::property_tree::json_parser_error& ex)
    {
        std::cout << "Message:" << ex.message() << std::endl;
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
        _mapHeight = layer.second.get<int>("height");
        _mapWidth = layer.second.get<int>("width");

        _tilePositions.reserve(_mapHeight * _mapWidth);

        // It is boost::optional<ptree&>
        auto data  = layer.second.get_child_optional("data");
        // No data in layer.
        if (!data)
        {
            return false;
        }

        for (auto& dataElement : data.get())
        {
            _tilePositions.push_back(std::stoi(dataElement.second.data()));
        }

        // Don't need more than 1 layer.
        break;
    }

    return IsValidMap();
}

}
