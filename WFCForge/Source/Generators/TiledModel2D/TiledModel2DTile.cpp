#pragma once
#include "Ge"

namespace WFCForge
{
    class TiledModel2DTile
    {
    public:
        TiledModel2DTile(int tileWidth, int tileHeight);
        virtual ~TiledModel2DTile();

        int SetData(int width, int height, unsigned char* data, bool isDataRGB = false);
        void UploadToGPU();
        Texture2D& GetGPUTexture();

    private:
        void ApplyTileResolution();
        void ApplyTilemapSize();

    private:
        Texture2D tex;
        std::vector<unsigned char> data;
        std::array<std::vector<unsigned char>, 4> faceIDs; // top right bottom left
        uint32_t tileHash = 0;
    };
}