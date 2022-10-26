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
        void UpdateTextureIdMap();

    private:
        TiledModel2DTilemap tilemap;
        TiledModel2DTileset tileset;
        Texture2D viewportTexture;
        int tileResolution[2] = {10, 10};
        int tileMapSize[2] = {10, 10};
        std::unordered_map<int, ImTextureID> texIds;
        int tileToCollapse[2] = { 0, 0 };
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