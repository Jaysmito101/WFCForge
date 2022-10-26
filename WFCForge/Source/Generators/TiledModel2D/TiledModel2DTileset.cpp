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

    void TiledModel2DTileset::AutoCalculateTileNeighbours()
    {
        int myHash[4] = {};
        int otHash[4] = {};
        for (auto a = 0 ; a < hashes.size() ; a++)
        {
            for (auto i = 0; i < 4; i++)
            {
                tiles[hashes[a]].allowedTiles[i].clear();
                myHash[i] = tiles[hashes[a]].GetFaceIDHash(i);
            }
            for (auto b = 0; b < hashes.size(); b++)
            {
                int otTileHash = hashes[b];
                for (auto i = 0; i < 4; i++) otHash[i] = tiles[hashes[b]].GetFaceIDHash(i);
                if (myHash[WFC_TM2D_TILE_TOP] == otHash[WFC_TM2D_TILE_BOTTOM]) tiles[hashes[a]].allowedTiles[WFC_TM2D_TILE_TOP].push_back(otTileHash);
                if (myHash[WFC_TM2D_TILE_RIGHT] == otHash[WFC_TM2D_TILE_LEFT]) tiles[hashes[a]].allowedTiles[WFC_TM2D_TILE_RIGHT].push_back(otTileHash);
                if (myHash[WFC_TM2D_TILE_BOTTOM] == otHash[WFC_TM2D_TILE_TOP]) tiles[hashes[a]].allowedTiles[WFC_TM2D_TILE_BOTTOM].push_back(otTileHash);
                if (myHash[WFC_TM2D_TILE_LEFT] == otHash[WFC_TM2D_TILE_RIGHT]) tiles[hashes[a]].allowedTiles[WFC_TM2D_TILE_LEFT].push_back(otTileHash);
            }
        }
    }

    void TiledModel2DTileset::RemoveTile(int hash)
    {
        if (tiles.find(hash) == tiles.end()) return;
        tiles.erase(hash);
        for (auto i = 0; i < hashes.size(); i++)
        {
            if (hashes[i] == hash)
            {
                hashes.erase(hashes.begin() + i);
                break;
            }
        }
    }

    void TiledModel2DTileset::AddTile(TiledModel2DTile tile)
    {
        if (tiles.find(tile.GetTileHash()) != tiles.end()) return;
        tiles[tile.GetTileHash()] = tile;
        if (!tiles[tile.GetTileHash()].IsUploadedToGPU()) tiles[tile.GetTileHash()].UploadToGPU();
        hashes.push_back(tile.GetTileHash());
    }
    
}