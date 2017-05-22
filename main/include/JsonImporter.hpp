#pragma once

#include <string>
#include <vector>

namespace Service
{

class JsonImporter
{
public:
    bool LoadMap(const std::string&);

public:
    inline int GetMapHeight() const;
    inline int GetMapWidth() const;
    inline int GetTileHeight() const;
    inline int GetTileWidth() const;

private:
    inline bool IsValidMap() const;

private:
    int _mapHeight = -1;
    int _mapWidth = -1;
    int _tileHeight = -1;
    int _tileWidth = -1;
    std::vector<int> _tilePositions;
    std::string _orientation;
    std::string _renderorder;

    const std::string _ORIENTATION = "orthogonal";
    const std::string _RENDERORDER = "left-up";
};

}
