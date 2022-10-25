#pragma once
#include "Generators/TiledModel2D/TiledModel2DTilemap.hpp"

namespace WFCForge
{
    TiledModel2DTilemap::TiledModel2DTilemap()
    {
    }

    TiledModel2DTilemap::~TiledModel2DTilemap()
    {
    }

    void TiledModel2DTilemap::Clean()
    {
        this->isPrepared = false;
    }

    void TiledModel2DTilemap::BakeToTexture(Texture2D* tex)
    {
        for (int i = 0; i < countY; i++)
        {
            for (int j = 0; j < countX; j++)
            {
                tex->UploadData(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY, tiles[i * countX + j].Back()->GetDataPTR());
            }
        }
    }
    
    void TiledModel2DTilemap::Prepare(int countX_, int countY_, int tileSizeX_, int tileSizeY_, TiledModel2DTileset* tileset)
    {
        this->countX = countX_;
        this->countY = countY_;
        this->tileSizeX = tileSizeX_;
        this->tileSizeY = tileSizeY_;

        this->tiles.clear();
        this->tiles = std::vector<TiledModel2DTileset>(countX * countY);
        for (int i = 0; i < tiles.size(); i++) tileset->Clone(&tiles[i]);

        this->isPrepared = true;
    }
}