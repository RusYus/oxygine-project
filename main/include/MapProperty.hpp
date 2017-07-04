#pragma once

#include <string>
#include <vector>

struct MapObject
{
    int mWidth = -1;
    int mHeight = -1;
    int mX = -1;
    int mY = -1;
    int mId = -1;
};

struct MapProperty
{
    inline bool IsValidTilesLayer() const
    {
        return (mMapHeight > 0 && mMapWidth > 0 && mTileHeight > 0 && mTileWidth > 0
            && mTilesPositions.size() == mMapHeight * mMapWidth);
    }

    inline bool IsValidObjectsLayer() const
    {
        const auto& invalidObject = std::find_if(mObjects.cbegin(), mObjects.cend(),
            [] (const MapObject& object)
            {
                return ( object.mWidth <= 0 || object.mHeight <= 0
                    || object.mX < 0 || object.mY < 0
                    || object.mId < 0);
            });

        return (invalidObject == mObjects.cend());
    }

    int mMapWidth = -1;
    int mMapHeight = -1;
    int mTileWidth = -1;
    int mTileHeight = -1;
    int mTilesetCols = -1;
    int mTilesetRows = -1;
    std::vector<int> mTilesPositions;
    std::vector<MapObject> mObjects;
};
