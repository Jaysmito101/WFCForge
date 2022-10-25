#pragma once
#include "Generators/TiledModel2D/TiledModel2DTileset.hpp"

namespace WFCForge
{
    TiledModel2DTileset::TiledModel2DTileset()
    {
    }

    TiledModel2DTileset::~TiledModel2DTileset()
    {
    }

    void TiledModel2DTileset::Clone(TiledModel2DTileset* other)
    {
        other->tiles = this->tiles;
    }

    void TiledModel2DTileset::AddTile(TiledModel2DTile tile)
    {
        for (auto i = 0; i < tiles.size(); i++)
        {
            if (tiles[i] == tile)  // Fixme: Possibility of hash collisions
                return;
        }
        tiles.push_back(tile);
    }
    
    void TiledModel2DTileset::UploadTilesToGPU()
    {
        for (auto i = 0 ; i < tiles.size() ; i++)
        {
            if (!tiles[i].IsUploadedToGPU())
                tiles[i].UploadToGPU();
        }
    }
}