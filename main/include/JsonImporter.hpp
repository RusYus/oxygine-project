#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

namespace Service
{

struct MapObject
{
    int _width = -1;
    int _height = -1;
    int _x = -1;
    int _y = -1;
    int _id = -1;
};

class JsonImporter
{
public:
    JsonImporter() = default;
    bool LoadMap(const std::string&);

public:
    inline int GetMapHeight() const
    {
        return _mapHeight;
    }

    inline int GetMapWidth() const
    {
        return _mapWidth;
    }

    inline int GetTileHeight() const
    {
        return _tileHeight;
    }

    inline int GetTileWidth() const
    {
        return _tileWidth;
    }

    std::vector<int> _tilePositions;
private:
    inline bool IsValidMap() const;
    inline bool IsValidTilesLayer() const;
    inline bool IsValidObjectsLayer() const;
    template<typename T>
    bool LoadTilesLayer(const T&);
    template<typename T>
    bool LoadObjectsLayer(const T&);

private:
    int _mapHeight = -1;
    int _mapWidth = -1;
    int _tileHeight = -1;
    int _tileWidth = -1;
    std::vector<MapObject> _objects;
    std::string _orientation;
    std::string _renderorder;

    const std::string _ORIENTATION = "orthogonal";
    const std::string _RENDERORDER = "left-up";
};

}
