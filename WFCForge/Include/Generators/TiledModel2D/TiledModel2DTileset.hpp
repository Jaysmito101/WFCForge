#pragma once
#include "Graphics/Texture2D.hpp"

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
        Texture2D viewportTexture;
        int tileResolution[2] = {10, 10};
        int tileMapSize[2] = {10, 10};
    };
}