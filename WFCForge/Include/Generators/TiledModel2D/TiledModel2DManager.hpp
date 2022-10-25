#pragma once
#include "Graphics/Texture2D.hpp"
#include "Generators/TiledModel2D/TiledModel2DTilemap.hpp"

namespace WFCForge
{
    class TiledModel2DManager
    {
    public:
        TiledModel2DManager() = default;
        virtual ~TiledModel2DManager() = default;


        void Setup();
        void Destroy();
        void Update();

        void* GetViewportTexture();        

        void ShowSettings();

    private:
        void ApplyTileResolution();
        void ApplyTilemapSize();

    private:
        TiledModel2DTilemap tilemap;
        TiledModel2DTileset tileset;
        Texture2D viewportTexture;
        int tileResolution[2] = {10, 10};
        int tileMapSize[2] = {10, 10};
        struct
        {
            Texture2D tex;
            unsigned char* data = NULL;
            int h = 0, w = 0;
            bool addRotated = true;
            bool isRGB = false;
        } tileToAdd;
    };
}