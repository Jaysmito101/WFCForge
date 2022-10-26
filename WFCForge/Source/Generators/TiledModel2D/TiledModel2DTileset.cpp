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

    void TiledModel2DTileset::AddTile(TiledModel2DTile tile, bool checkDuplicates)
    {
        if (checkDuplicates)
        {
            for (auto i = 0; i < tiles.size(); i++)
            {
                if (tiles[i] == tile)  // Fixme: Possibility of hash collisions
                    return;
            }
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

    void TiledModel2DTileset::Collapse(int index)
    {
        // TODO: maybe do this in some better way
        auto temp = tiles[index];
        tiles.clear();
        tiles.push_back(temp);
    }
}