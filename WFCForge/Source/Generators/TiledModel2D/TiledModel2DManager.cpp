#include "Generators/TiledModel2D/TiledModel2DManager.hpp"
#include "WFCForge.hpp"

#include "stb_image.h"

namespace WFCForge
{

    void TiledModel2DManager::Setup()
    {
        viewportTexture.CreateEmpty(100, 100);
    }

    void TiledModel2DManager::Destroy()
    {
        viewportTexture.Destroy();
        if (tileToAdd.tex.IsLoaded()) tileToAdd.tex.Destroy();
        if (tileToAdd.data) free(tileToAdd.data);
    }

    void TiledModel2DManager::Update()
    {
        
    }

    void TiledModel2DManager::ApplyTileResolution()
    {
        if (tilemap.IsPrepared()) tilemap.Clean();
        tileset.Clear();
        // TODO
    }

    void TiledModel2DManager::ApplyTilemapSize()
    {
        if (tilemap.IsPrepared()) tilemap.Clean();
        
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
                tileResolution[0] = tileResolutionCopy[0];
                tileResolution[1] = tileResolutionCopy[1];
                ApplyTileResolution();
            }
        }

        static int tileMapSizeCopy[2] = {10, 10};
        ImGui::InputInt2("Tilemap Size (Tile Count)", tileMapSize);
        if((tileMapSizeCopy[0] != tileMapSize[0]) || (tileMapSizeCopy[1] != tileMapSize[1]))
        {
            if(ImGui::Button("Apply Change"))
            {
                tileMapSize[0] = tileMapSizeCopy[0];
                tileMapSize[1] = tileMapSizeCopy[1];
                ApplyTilemapSize();
            }
        }

        if (ImGui::Button("Add Tile"))
        {
            tileToAdd.isRGB = false;
            std::string path = Utils::ShowFileOpenDialog();
            if (tileToAdd.tex.IsLoaded()) tileToAdd.tex.Destroy();
            if (tileToAdd.data) free(tileToAdd.data);
            int channels = 3;
            stbi_set_flip_vertically_on_load(false);
            tileToAdd.data = stbi_load(path.data(), &tileToAdd.w, &tileToAdd.h, &channels, 4);
            //if (channels == 3) tileToAdd.isRGB = true;
            if (tileToAdd.data != nullptr) tileToAdd.tex.LoadFromMemory(tileToAdd.w, tileToAdd.h, tileToAdd.data);
            ImGui::OpenPopup("##AddNewTileTexture");
        }

        ImGui::Text("Tiles in Tileset");
        ImGui::BeginChild("Tilesets", ImVec2(0, 250));
        for (auto i = 0; i < tileset.tiles.size(); i++)
        {
            //ImGui::Image((ImTextureID)(intptr_t)tileset.tiles[i].GetGPUTexture().GetHandle(), ImVec2((float)tileResolution[0], (float)tileResolution[1]));
            ImGui::Image((ImTextureID)(intptr_t)tileset.tiles[i].GetGPUTexture().GetHandle(), ImVec2(100, 100));
            static char buff[256];
            sprintf(buff, "##TilesetTilePreview %d", i);
            if (ImGui::BeginPopupContextItem(buff))
            {
                ImGui::InputInt("Top ID", tileset.tiles[i].GetFaceIDHashPTR(0));
                ImGui::InputInt("Right ID", tileset.tiles[i].GetFaceIDHashPTR(1));
                ImGui::InputInt("Bottom ID", tileset.tiles[i].GetFaceIDHashPTR(2));
                ImGui::InputInt("Left ID", tileset.tiles[i].GetFaceIDHashPTR(3));
                ImGui::Text("Hash: %u", tileset.tiles[i].GetTileHash());
                if (ImGui::Button("Delete"))
                {
                    tileset.tiles.erase(tileset.tiles.begin() + i);
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    break;
                }
                ImGui::EndPopup();
            }
            if ((i + 1) % 3 != 0) ImGui::SameLine();
        }
        ImGui::NewLine();
        ImGui::EndChild();

        if (!tilemap.IsPrepared())
        {
            if (ImGui::Button("Prepare Tilemap"))
            {
                tilemap.Prepare(tileMapSize[0], tileMapSize[1], tileResolution[0], tileResolution[1], &tileset);
                if (viewportTexture.IsLoaded()) viewportTexture.Destroy();
                viewportTexture.CreateEmpty(tileMapSize[0] * tileResolution[0], tileMapSize[1] * tileResolution[1]);
                tilemap.BakeToTexture(&viewportTexture);
            }
        }
        
        static int tileToCollapse[2] = {0, 0};
        if (tilemap.IsPrepared())
        {
            tileToCollapse[0] = std::clamp(tileToCollapse[0], 0, tileMapSize[0]);
            tileToCollapse[1] = std::clamp(tileToCollapse[1], 0, tileMapSize[1]);
            ImGui::DragInt("##TileToCollapseIndexX", &tileToCollapse[0], 0.1f, 0, tileMapSize[0]);
            ImGui::DragInt("##TileToCollapseIndexY", &tileToCollapse[1], 0.1f, 0, tileMapSize[1]);
            if (ImGui::Button("Collapse"))
                ImGui::OpenPopup("##TileToCollapsePopup");
            if (ImGui::Button("Clean"))
                tilemap.Clean();
        }

        if (ImGui::BeginPopup("##AddNewTileTexture"))
        {
            if (!tileToAdd.tex.IsLoaded()) ImGui::Text("Failed to load texture");
            else
            {
                ImGui::Image((ImTextureID)(intptr_t)tileToAdd.tex.GetHandle(), ImVec2(100, 100));
                if(tileToAdd.isRGB) ImGui::Text("Texture will be conveted to RGBA forcefully on add");
                ImGui::Checkbox("Add Rotated Versions", &tileToAdd.addRotated);
                if (ImGui::Button("Add"))
                {
                    TiledModel2DTile tile(tileResolution[0], tileResolution[1]);
                    tile.SetData(tileToAdd.w, tileToAdd.h, tileToAdd.data, tileToAdd.isRGB);
                    tileset.AddTile(tile);
                    if (tileToAdd.addRotated)
                    {
                        tile.Rotate(1); tileset.AddTile(tile);
                        tile.Rotate(1); tileset.AddTile(tile);
                        tile.Rotate(1); tileset.AddTile(tile);
                    }
                    free(tileToAdd.data); tileToAdd.data = nullptr;
                    tileset.UploadTilesToGPU();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("##TileToCollapsePopup"))
        {
            auto tileIndex = tileToCollapse[1] * tileMapSize[0] + tileToCollapse[0];
            auto size = tilemap.tiles[tileIndex].tiles.size();
            if (size == 1) ImGui::Text("Tile already collapsed");
            else
            {
                ImGui::Text("Collapse Tile With");
                for (auto i = 0; i < size; i++)
                {
                    if (ImGui::ImageButton((ImTextureID)(intptr_t)tilemap.tiles[tileIndex].tiles[i].GetGPUTexture().GetHandle(), ImVec2(100, 100)))
                    {
                        tilemap.Collapse(tileToCollapse[0], tileToCollapse[1], i);
                        tilemap.BakeToTexture(&viewportTexture);
                        ImGui::CloseCurrentPopup();
                        break;
                    }
                    if ((i + 1) % 3 != 0) ImGui::SameLine();
                }
                ImGui::NewLine();
            }
            ImGui::EndPopup();
        }
                
    }

}