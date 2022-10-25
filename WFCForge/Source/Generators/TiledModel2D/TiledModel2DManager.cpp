#include "Generators/TiledModel2D/TiledModel2DManager.hpp"
#include "WFCForge.hpp"


namespace WFCForge
{

    void TiledModel2DManager::Setup()
    {
        viewportTexture.CreateEmpty(100, 100);
    }

    void TiledModel2DManager::Destroy()
    {
        viewportTexture.Destroy();
    }

    void TiledModel2DManager::Update()
    {
        
    }

    void TiledModel2DManager::ApplyTileResolution()
    {

    }

    void ApplyTilemapSize()
    {

    }

    void* TiledModel2DManager::GetViewportTexture()
    {
        return (void*)(intptr_t)viewportTexture.GetHandle();
    }

    void TiledModel2DManager::ShowSettings()
    {
        static int tileResolutionCopy[2] = {10, 10};
        ImGui::InputInt2("Tile Resolution", tileResolutionCopy);
        if((tileResolutionCopy[0] != tileResolution[0]) || (tileResolutionCopy[1] != tileResolution[1]))
        {
            if(ImGui::Button("Apply Change"))
            {
                tileResolutionCopy[0] = tileResolution[0];
                tileResolutionCopy[1] = tileResolution[1];
                ApplyTileResolution();
            }
        }

        static int tileMapSizeCopy[2] = {10, 10};
        ImGui::InputInt2("Tilemap Size (Tile Count)", tileMapSize);
        if((tileMapSizeCopy[0] != tileMapSize[0]) || (tileMapSizeCopy[1] != tileMapSize[1]))
        {
            if(ImGui::Button("Apply Change"))
            {
                tileMapSizeCopy[0] = tileMapSize[0];
                tileMapSizeCopy[1] = tileMapSize[1];
                ApplyTilemapSize();
            }
        }
                
    }

}