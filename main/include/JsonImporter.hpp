#pragma once

#include <string>
#include <vector>

namespace Service
{

class JsonImporter
{
public:
    bool LoadMap();

public:
    inline int GetMapHeight() const;
    inline int GetMapWidth() const;
    inline int GetTileHeight() const;
    inline int GetTileWidth() const;

private:
    inline bool IsValidMap() const;

private:
    int _mapHeight;
    int _mapWidth;
    int _tileHeight;
    int _tileWidth;
    std::vector<int> _tilePositions;
    std::string _orientation;
    std::string _renderorder;

    const std::string _ORIENTATION = "orthogonal";
    const std::string _RENDERORDER = "left-up";
};

}
