#include "Generators/TiledModel2D/TiledModel2DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

#include "stb_image.h"

namespace WFCForge
{

	void TiledModel2DManager::Setup(AppState* appState)
	{
		viewportTexture.CreateEmpty(100, 100);
		this->appState = appState;
	}

	void TiledModel2DManager::Destroy()
	{
		viewportTexture.Destroy();
		if (tileToAdd.tex.IsLoaded()) tileToAdd.tex.Destroy();
		if (blank.IsLoaded()) blank.Destroy();
		if (tileToAdd.data) free(tileToAdd.data);
	}

	void TiledModel2DManager::Update()
	{

	}

	void TiledModel2DManager::ApplyTileResolution()
	{
		if (tilemap.IsPrepared()) tilemap.Clean();
		tileset.Clear();
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
		if(!blank.IsLoaded()) blank.CreateEmpty(tileResolution[0], tileResolution[1]);

		static int tileResolutionCopy[2] = { 10, 10 };
		ImGui::InputInt2("Tile Resolution", tileResolutionCopy);
		if ((tileResolutionCopy[0] != tileResolution[0]) || (tileResolutionCopy[1] != tileResolution[1]))
		{
			if (ImGui::Button("Apply Change"))
			{
				tileResolution[0] = tileResolutionCopy[0];
				tileResolution[1] = tileResolutionCopy[1];
				ApplyTileResolution();
			}
		}

		static int tileMapSizeCopy[2] = { 10, 10 };
		ImGui::InputInt2("Tilemap Size (Tile Count)", tileMapSizeCopy);
		if ((tileMapSizeCopy[0] != tileMapSize[0]) || (tileMapSizeCopy[1] != tileMapSize[1]))
		{
			if (ImGui::Button("Apply Change"))
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
		if (ImGui::Button("Clear Tiles")) tileset.Clear();
		if (ImGui::Button("Auto Configure Tiles Neighbours")) tileset.AutoCalculateTileNeighbours();

		static bool openConfigNeighboursModal = false;

		ImGui::Text("Tiles in Tileset");
		ImGui::BeginChild("Tilesets", ImVec2(0, 250));
		for (auto i = 0; i < tileset.hashes.size(); i++)
		{
			ImGui::Image((ImTextureID)(intptr_t)tileset.tiles[tileset.hashes[i]].GetGPUTexture().GetHandle(), ImVec2(100, 100));
			static char buff[256];
			sprintf(buff, "##TilesetTilePreview %d", i);
			if (ImGui::BeginPopupContextItem(buff))
			{
				ImGui::InputInt("Top ID", tileset.tiles[tileset.hashes[i]].GetFaceIDHashPTR(0));
				ImGui::InputInt("Right ID", tileset.tiles[tileset.hashes[i]].GetFaceIDHashPTR(1));
				ImGui::InputInt("Bottom ID", tileset.tiles[tileset.hashes[i]].GetFaceIDHashPTR(2));
				ImGui::InputInt("Left ID", tileset.tiles[tileset.hashes[i]].GetFaceIDHashPTR(3));
				ImGui::Text("Hash: %u", tileset.tiles[tileset.hashes[i]].GetTileHash());
				if (ImGui::Button("Configure Neighbours"))
				{
					tileToConfigureNeighbours = tileset.hashes[i];
					openConfigNeighboursModal = true;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete"))
				{
					tileset.RemoveTile(tileset.hashes[i]);
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

		if (openConfigNeighboursModal)
		{
			openConfigNeighboursModal = false;
			ImGui::OpenPopup("##TileConfigureNewghbours");
		}

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

		static int seed = 42;
		ImGui::DragInt("Seed", &seed);

		if (tilemap.IsPrepared())
		{
			tileToCollapse[0] = std::clamp(tileToCollapse[0], 0, tileMapSize[0]);
			tileToCollapse[1] = std::clamp(tileToCollapse[1], 0, tileMapSize[1]);
			ImGui::DragInt("##TileToCollapseIndexX", &tileToCollapse[0], 0.1f, 0, tileMapSize[0]);
			ImGui::DragInt("##TileToCollapseIndexY", &tileToCollapse[1], 0.1f, 0, tileMapSize[1]);
			if (ImGui::Button("Collapse"))
				ImGui::OpenPopup("##TileToCollapsePopup");

			if (ImGui::Button("Auto Collapse Next"))
			{
				tilemap.CollapseNext();
				tilemap.BakeToTexture(&viewportTexture);
			}

			if (ImGui::Button("Auto Collapse All"))
			{
				tilemap.CollapseAll(seed);
				tilemap.BakeToTexture(&viewportTexture);
			}

			if (ImGui::Button("Clean")) tilemap.Clean();

			if (appState->mouseButton.right && appState->mousePosition.x >= 0.0f && appState->mousePosition.y >= 0.0f)
			{
				tileToCollapse[0] = std::clamp((int)(appState->mousePosition.x * (tileResolution[0] * tileMapSize[0] - 1)) / tileResolution[0], 0, tileMapSize[0] - 1);
				tileToCollapse[1] = std::clamp((int)(appState->mousePosition.y * (tileResolution[1] * tileMapSize[1] - 1)) / tileResolution[1], 0, tileMapSize[1] - 1);
				ImGui::OpenPopup("##TileToCollapsePopup");
			}

			if (ImGui::Button("Export Image"))
			{
				auto exportPath = Utils::ShowSaveFileDialog();
				if (exportPath.size() > 3) tilemap.ExportImage(exportPath);
			}
		}


		if (ImGui::BeginPopup("##AddNewTileTexture"))
		{
			if (!tileToAdd.tex.IsLoaded()) ImGui::Text("Failed to load texture");
			else
			{
				ImGui::Image((ImTextureID)(intptr_t)tileToAdd.tex.GetHandle(), ImVec2(100, 100));
				if (tileToAdd.isRGB) ImGui::Text("Texture will be conveted to RGBA forcefully on add");
				if (tileResolution[0] == tileResolution[1]) ImGui::Checkbox("Add Rotated Versions", &tileToAdd.addRotated);
				if (ImGui::Button("Add"))
				{
					TiledModel2DTile tile(tileResolution[0], tileResolution[1]);
					tile.SetData(tileToAdd.w, tileToAdd.h, tileToAdd.data, tileToAdd.isRGB);
					tileset.AddTile(tile);
					if (tileResolution[0] == tileResolution[1])
					{
						if (tileToAdd.addRotated)
						{
							tile.Rotate(1); tileset.AddTile(tile);
							tile.Rotate(1); tileset.AddTile(tile);
							tile.Rotate(1); tileset.AddTile(tile);
						}
					}
					free(tileToAdd.data); tileToAdd.data = nullptr;
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}


		if (ImGui::BeginPopup("##TileToCollapsePopup"))
		{
			ShowTileToCollapsePopup();
			ImGui::EndPopup();
		}


		if (ImGui::BeginPopupModal("##TileConfigureNewghbours"))
		{
			ShowTileConfigureNeihbours();
			ImGui::EndPopup();
		}
	}

	void TiledModel2DManager::ShowTileToCollapsePopup()
	{
		auto tileIndex = tileToCollapse[1] * tileMapSize[0] + tileToCollapse[0];
		auto& vec = tilemap.tiles[tileIndex];
		if (vec.size() <= 1) ImGui::Text("Tile already collapsed");
		else
		{
			int i = 0;
			for (auto tileHash : vec)
			{
				if (ImGui::ImageButton((ImTextureID)(intptr_t)tileset.tiles[tileHash].GetGPUTexture().GetHandle(), ImVec2(50, 50)))
				{
					tilemap.Collapse(tileToCollapse[0], tileToCollapse[1], i);
					tilemap.BakeToTexture(&viewportTexture);
					ImGui::CloseCurrentPopup();
					break;
				}
				i++;
				if (i % 3 != 0) ImGui::SameLine();
			}
		}
		ImGui::NewLine();
		ImGui::Text("Force Set tile to");
		auto& allowedtilesList = tileset.hashes;
		for (auto i = 0; i < allowedtilesList.size(); i++)
		{
			if (ImGui::ImageButton((ImTextureID)(intptr_t)tileset.tiles[allowedtilesList[i]].GetGPUTexture().GetHandle(), ImVec2(50, 50)))
			{
				tilemap.tiles[tileToCollapse[1] * tileMapSize[0] + tileToCollapse[0]].clear();
				tilemap.tiles[tileToCollapse[1] * tileMapSize[0] + tileToCollapse[0]].push_back(allowedtilesList[i]);
				tilemap.BakeToTexture(&viewportTexture);
				ImGui::CloseCurrentPopup();
			}
			if ((i + 1) % 3 != 0) ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	void TiledModel2DManager::ShowTileConfigureNeihbours()
	{
		static int allotedSide = -1;
		ImTextureID blankId = (ImTextureID)(intptr_t)blank.GetHandle();
		ImTextureID toID = nullptr;
		auto& allotedTile = tileset.tiles[tileToConfigureNeighbours];
		ImGui::Dummy(ImVec2(50, 50)); ImGui::SameLine();
		// top

		if (allotedSide == -1)
		{
			toID = (allotedTile.allowedTiles[WFC_TM2D_TILE_TOP].size() > 0) ? (ImTextureID)(intptr_t)tileset.tiles[allotedTile.allowedTiles[WFC_TM2D_TILE_TOP].back()].GetGPUTexture().GetHandle() : blankId;
			ImGui::PushID(WFC_TM2D_TILE_TOP);
			if (ImGui::ImageButton(toID, ImVec2(50, 50)))  allotedSide = WFC_TM2D_TILE_TOP;
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(50, 50));

			// left
			toID = (allotedTile.allowedTiles[WFC_TM2D_TILE_LEFT].size() > 0) ? (ImTextureID)(intptr_t)tileset.tiles[allotedTile.allowedTiles[WFC_TM2D_TILE_LEFT].back()].GetGPUTexture().GetHandle() : blankId;
			ImGui::PushID(WFC_TM2D_TILE_LEFT);
			if (ImGui::ImageButton(toID, ImVec2(50, 50)))  allotedSide = WFC_TM2D_TILE_LEFT;
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::Image((ImTextureID)(intptr_t)tileset.tiles[tileToConfigureNeighbours].GetGPUTexture().GetHandle(), ImVec2(50, 50));  ImGui::SameLine();
			toID = (allotedTile.allowedTiles[WFC_TM2D_TILE_RIGHT].size() > 0) ? (ImTextureID)(intptr_t)tileset.tiles[allotedTile.allowedTiles[WFC_TM2D_TILE_RIGHT].back()].GetGPUTexture().GetHandle() : blankId;
			ImGui::PushID(WFC_TM2D_TILE_RIGHT);
			if (ImGui::ImageButton(toID, ImVec2(50, 50)))  allotedSide = WFC_TM2D_TILE_RIGHT;
			ImGui::PopID();


			ImGui::Dummy(ImVec2(50, 50)); ImGui::SameLine();
			toID = (allotedTile.allowedTiles[WFC_TM2D_TILE_BOTTOM].size() > 0) ? (ImTextureID)(intptr_t)tileset.tiles[allotedTile.allowedTiles[WFC_TM2D_TILE_BOTTOM].back()].GetGPUTexture().GetHandle() : blankId;
			ImGui::PushID(WFC_TM2D_TILE_BOTTOM);
			if (ImGui::ImageButton(toID, ImVec2(50, 50))) allotedSide = WFC_TM2D_TILE_BOTTOM;
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(50, 50));
		}
		else if (allotedSide >= 0 && allotedSide <= 3)
		{
			if (ImGui::Button("Back")) { allotedSide = -1; return; }
			ImGui::Image((ImTextureID)(intptr_t)tileset.tiles[tileToConfigureNeighbours].GetGPUTexture().GetHandle(), ImVec2(50, 50));
			ImGui::Text("Allowed Tiles");
			auto allowedtilesList = allotedTile.allowedTiles[allotedSide];
			if (allowedtilesList.size() == 0) ImGui::Text("None");
			for (auto i = 0; i < allowedtilesList.size(); i++)
			{
				if (ImGui::ImageButton((ImTextureID)(intptr_t)tileset.tiles[allowedtilesList[i]].GetGPUTexture().GetHandle(), ImVec2(50, 50)))
				{
					tileset.tiles[tileToConfigureNeighbours].allowedTiles[allotedSide].erase(tileset.tiles[tileToConfigureNeighbours].allowedTiles[allotedSide].begin() + i);
					break;
				}
				if ((i + 1) % 3 != 0) ImGui::SameLine();
			}
			ImGui::NewLine();
			ImGui::Text("Select Tiles");
			allowedtilesList = tileset.hashes;
			for (auto i = 0; i < allowedtilesList.size(); i++)
			{
				if (ImGui::ImageButton((ImTextureID)(intptr_t)tileset.tiles[allowedtilesList[i]].GetGPUTexture().GetHandle(), ImVec2(50, 50)))
				{
					tileset.tiles[tileToConfigureNeighbours].allowedTiles[allotedSide].push_back(allowedtilesList[i]);
					break;
				}
				if ((i + 1) % 3 != 0) ImGui::SameLine();
			}
			ImGui::NewLine();
		}

		if (ImGui::Button("Ok")) ImGui::CloseCurrentPopup();
		
	}

}