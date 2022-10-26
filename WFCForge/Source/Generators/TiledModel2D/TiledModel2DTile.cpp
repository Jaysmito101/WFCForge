#pragma once
#include "Generators/TiledModel2D/TiledModel2DTile.hpp"
#include "avir.h"

namespace WFCForge
{
    TiledModel2DTile::TiledModel2DTile(const TiledModel2DTile& other)
    {
        this->width = other.width;
        this->height = other.height;
        this->data = other.data;
        this->faceIDHashes = other.faceIDHashes;
        this->faceIDs = faceIDs;
        this->tileHash = other.tileHash;
        this->allowedTiles = other.allowedTiles;
    }

    TiledModel2DTile::TiledModel2DTile(int tileWidth, int tileHeight)
    {
        this->height = tileHeight;
        this->width = tileWidth;
        this->data.clear();
        for (auto i = 0; i < tileHeight * tileWidth * 4; i++) data.push_back(0);
    }

    TiledModel2DTile::~TiledModel2DTile()
    {
        if(tex.IsLoaded()) tex.Destroy();
    }

    int TiledModel2DTile::SetData(int w, int h, unsigned char* d, bool isDataRGB)
    {
        if (isDataRGB) throw std::exception("RGB images not supported!");
        if (w == width && h == height)
            memcpy(data.data(), d, w * h * 4);
        else
        {
            avir::CImageResizer<> ImageResizer(8);
            ImageResizer.resizeImage(d, w, h, 0, data.data(), width, height, 4, 0);
        }
        CalculateFaceIDs();
        return 0;
    }

    void TiledModel2DTile::UploadToGPU()
    {
        if (width * height * 4 != data.size()) return;
        if (tex.IsLoaded()) tex.Destroy();
        tex.LoadFromMemory(width, height, data.data());
    }

    Texture2D& TiledModel2DTile::GetGPUTexture()
    {
        return this->tex;
    }

    bool TiledModel2DTile::IsTileAllowed(int newTileHash, int position)
    {
        position = std::clamp(position, 0, 4);
        return std::find(allowedTiles[position].begin(), allowedTiles[position].end(), newTileHash) != allowedTiles[position].end();
    }
    
    void TiledModel2DTile::RotateOnce()
    {
        int newWidth = height;
        int newHeight = width;
        auto oldData = data;
        for (auto i = 0; i < this->height; i++)
        {
            for (auto j = 0; j < this->width; j++)
            {
                // data(h - 1 - i, j) = oldData(j, i)
                data[(j * newWidth + (height - i - 1)) * 4 + 0] = oldData[(i * width + j) * 4 + 0];
                data[(j * newWidth + (height - i - 1)) * 4 + 1] = oldData[(i * width + j) * 4 + 1];
                data[(j * newWidth + (height - i - 1)) * 4 + 2] = oldData[(i * width + j) * 4 + 2];
                data[(j * newWidth + (height - i - 1)) * 4 + 3] = oldData[(i * width + j) * 4 + 3];
            }
        }
        width = newWidth;
        height = newHeight;
        CalculateFaceIDs();
    }
    
    void TiledModel2DTile::CalculateFaceIDs()
    {
        // top
        faceIDs[0].clear();
        for (int i = 0; i < width; i++)
        {
            faceIDs[0].push_back(data[i * 4 + 0]);
            faceIDs[0].push_back(data[i * 4 + 1]);
            faceIDs[0].push_back(data[i * 4 + 2]);
            faceIDs[0].push_back(data[i * 4 + 3]);
        }
        // faceIDStrs[0] = Utils::ToHexString(faceIDs[0].data(), faceIDs[0].size());
        faceIDHashes[0] = (int32_t)Utils::Hash(faceIDs[0].data(), faceIDs[0].size());

        // right
        faceIDs[1].clear();
        for (int i = 0; i < height; i++)
        {
            faceIDs[1].push_back(data[(i * width + (width - 1)) * 4 + 0]);
            faceIDs[1].push_back(data[(i * width + (width - 1)) * 4 + 1]);
            faceIDs[1].push_back(data[(i * width + (width - 1)) * 4 + 2]);
            faceIDs[1].push_back(data[(i * width + (width - 1)) * 4 + 3]);
        }
        // faceIDStrs[1] = Utils::ToHexString(faceIDs[1].data(), faceIDs[1].size());
        faceIDHashes[1] = (int32_t)Utils::Hash(faceIDs[1].data(), faceIDs[1].size());

        // bottom
        faceIDs[2].clear();
        for (int i = 0; i < width; i++)
        {
            faceIDs[2].push_back(data[((height - 1) * width + i) * 4 + 0]);
            faceIDs[2].push_back(data[((height - 1) * width + i) * 4 + 1]);
            faceIDs[2].push_back(data[((height - 1) * width + i) * 4 + 2]);
            faceIDs[2].push_back(data[((height - 1) * width + i) * 4 + 3]);
        }
        // faceIDStrs[2] = Utils::ToHexString(faceIDs[2].data(), faceIDs[2].size());
        faceIDHashes[2] = (int32_t)Utils::Hash(faceIDs[2].data(), faceIDs[2].size());

        // left
        faceIDs[3].clear();
        for (int i = 0; i < height; i++)
        {
            faceIDs[3].push_back(data[(i * width + 0) * 4 + 0]);
            faceIDs[3].push_back(data[(i * width + 0) * 4 + 1]);
            faceIDs[3].push_back(data[(i * width + 0) * 4 + 2]);
            faceIDs[3].push_back(data[(i * width + 0) * 4 + 3]);
        }
        // faceIDStrs[3] = Utils::ToHexString(faceIDs[3].data(), faceIDs[3].size());
        faceIDHashes[3] = (int32_t)Utils::Hash(faceIDs[3].data(), faceIDs[3].size());

        this->tileHash = Utils::Hash(data.data(), data.size());
    }

    bool operator==(const TiledModel2DTile& lhs, const TiledModel2DTile& rhs)
    {
        return lhs.tileHash == rhs.tileHash;
    }
} 