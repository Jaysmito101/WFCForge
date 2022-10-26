#pragma once
#include "imgui.h"
#include "Graphics/Texture2D.hpp"
#include "Generators/TiledModel2D/TiledModel2DTilemap.hpp"

namespace WFCForge
{
    class AppState;

    class TiledModel2DManager
    {
    public:
        TiledModel2DManager() = default;
        virtual ~TiledModel2DManager() = default;

        void Setup(AppState* appState);
        void Destroy();
        void Update();

        void* GetViewportTexture();        

        void ShowSettings();

    private:
        void ApplyTileResolution();
        void ApplyTilemapSize();
        void ShowTileToCollapsePopup();
        void ShowTileConfigureNeihbours();

    private:
        TiledModel2DTilemap tilemap;
        TiledModel2DTileset tileset;
        Texture2D viewportTexture;
        Texture2D blank;
        int tileResolution[2] = {10, 10};
        int tileMapSize[2] = {10, 10};
        int tileToCollapse[2] = { 0, 0 };
        int tileToConfigureNeighbours = 0;
        AppState* appState = nullptr;
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