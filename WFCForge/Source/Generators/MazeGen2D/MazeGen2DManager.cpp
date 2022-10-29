#include "Generators/MazeGen2D/MazeGen2DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

#include "Generators/MazeGen2D/MazeGen2DDummy.hpp"
#include "Generators/MazeGen2D/MazeGen2DBinaryTree.hpp"
#include "Generators/MazeGen2D/MazeGen2DAldousBroder.hpp"
#include "Generators/MazeGen2D/MazeGen2DKruskal.hpp"
#include "Generators/MazeGen2D/MazeGen2DBacktracking.hpp"

#include <stb_image.h>
#include <stb_image_write.h>
#include <avir.h>

#define WALL_TILE_RAND(tile) currentTile = (wallTiles[tile].second.size() > 0) ? &(wallTiles[tile].second[rand() % wallTiles[tile].second.size()]) : &(wallTiles[tile].first)

namespace WFCForge
{
	static void ChangeTileFor(unsigned char* data, int width, int height)
	{
		
	}

	void MazeGen2DManager::Setup(AppState* appState)
	{
		this->appState = appState;
		this->viewportTile.Setup(100, 100);
		SetupDefaultTiles();
		this->algorithms.push_back(std::make_shared<MazeGen2DBinaryTree>());
		this->algorithms.push_back(std::make_shared<MazeGen2DAldousBroder>());
		this->algorithms.push_back(std::make_shared<MazeGen2DKruskal>());
		this->algorithms.push_back(std::make_shared<MazeGen2DBacktracking>());
		//this->algorithms.push_back(std::make_shared<MazeGen2DDummy>());
	}

	void MazeGen2DManager::Destroy()
	{
	}

	void MazeGen2DManager::Update()
	{

	}

	void MazeGen2DManager::ShowSettings()
	{
		if (!algorithms[selectedAlgorithm]->IsPrepared())
		{

			static int tileSizeCopy[2] = { 10, 10 };
			ImGui::DragInt2("Tile Size", tileSizeCopy, 0.1f, 1);
			if (tileSize[0] != tileSizeCopy[0] || tileSize[1] != tileSizeCopy[1])
			{
				if (ImGui::Button("Apply Changes"))
				{
					tileSize[0] = tileSizeCopy[0];
					tileSize[1] = tileSizeCopy[1];
					SetupDefaultTiles();
				}
			}

			ImGui::DragInt2("Tile Map Size", tileMapSize, 0.1f, 1);

			ShowChangeTileSettings();

			static const char* algorithmNames[] = {
				"Binary Tree",
				"Aldous Broder",
				"Kruskal",
				"Backtracking"
				// "Dummy"
			};


			if (ImGui::BeginCombo("Algorithm", algorithmNames[selectedAlgorithm]))
			{
				for (int i = 0; i < algorithms.size(); i++)
				{
					bool isSelected = (selectedAlgorithm == i);
					if (ImGui::Selectable(algorithmNames[i], isSelected))
						selectedAlgorithm = i;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			static int seed = 42;

			ImGui::DragInt("Seed", &seed);

			if (ImGui::Button("Prepare"))
			{
				viewportTile.Setup(tileMapSize[0] * tileSize[0], tileMapSize[1] * tileSize[1]);
				algorithms[selectedAlgorithm]->Setup(tileMapSize[0], tileMapSize[1], seed);
				UpdateViewportTexture();
			}

		}
		else
		{
			static bool generateAllOn = false;
			static bool visualiseGeneration = false;
			static float visualiseDelay = 100.0f;
			static float generateAllDelay = 0.0f;

			if (!generateAllOn)
			{
				if (ImGui::Button("Generate Step"))
				{
					algorithms[selectedAlgorithm]->Step();
					UpdateViewportTexture();
				}

				if (ImGui::Button("Generate")) generateAllOn = true;

			}
			
			ImGui::Checkbox("Enable Visualizer", &visualiseGeneration);			
			if (visualiseGeneration) ImGui::DragFloat("Visualizer Delay (in ms)", &visualiseDelay, 1.0f, 10.0f);
			if (generateAllOn) if (ImGui::Button("Cancel")) generateAllOn = false;

			if (!generateAllOn)
			{
				if (ImGui::Button("Clean")) algorithms[selectedAlgorithm]->Destroy();
				if (ImGui::Button("Export Image")) ExportImage();
				if (ImGui::Button("Export Path Mask")) ExportMask(false);
				if (ImGui::Button("Export Wall Mask")) ExportMask(true);
				if (ImGui::Button("Export As Text")) ExportText();
			}
			
			if (generateAllOn)
			{
				if (visualiseGeneration)
				{
					generateAllDelay += appState->deltaTime * 1000.0f;
					if (generateAllDelay >= visualiseDelay)
					{
						generateAllDelay = 0.0f;
						algorithms[selectedAlgorithm]->Step();
						UpdateViewportTexture();
					}
				}
				else algorithms[selectedAlgorithm]->Step();
			}

			auto generateAllOnCopy = generateAllOn;
			generateAllOn &= (!algorithms[selectedAlgorithm]->IsFinished());
			if (generateAllOn != generateAllOnCopy) UpdateViewportTexture();
			if (!generateAllOn) generateAllDelay = 0.0f;
		}

	}
	
	void MazeGen2DManager::ShowChangeTileSettings()
	{
		static char buff[1024];
		ImGui::BeginChild("##ChangeTileSettings", ImVec2(0, 200));
		ImGui::Text("Path Tiles : ");
		ImGui::Image(pathTiles.first.GetTexID(), ImVec2(50, 50));
		if (ImGui::BeginPopupContextItem("##PopUpForPAthTileSetting"))
		{
			for (auto j = 0; j < pathTiles.second.size(); j++)
			{
				ImGui::Image(pathTiles.second[j].GetTexID(), ImVec2(50, 50));
				sprintf(buff, "##PopUpForPAthTileSettingSub_%d", j);
				if (ImGui::BeginPopupContextItem(buff))
				{
					if(ImGui::Button("Rotate")) pathTiles.second[j].Rotate();
					if(ImGui::Button("Delete"))
					{
						pathTiles.second.erase(pathTiles.second.begin() + j);
						ImGui::EndPopup();
						break;
					}
					ImGui::EndPopup();
				}

				if ((j + 1) % 3 != 0) ImGui::SameLine();
			}
			ImGui::NewLine();
			if (ImGui::Button("Add"))
			{
				std::string path = Utils::ShowFileOpenDialog();
				if (path.size() > 3)
				{
					int w = 0, h = 0, ch = 4;
					stbi_set_flip_vertically_on_load(false);
					auto d = stbi_load(path.data(), &w, &h, &ch, 4);
					pathTiles.second.emplace_back();
					pathTiles.second.back().Setup(tileSize[0], tileSize[1]);
					pathTiles.second.back().SetData(w, h, d);
					stbi_image_free(d);
				}
			}
			ImGui::EndPopup();
		}
		ImGui::NewLine();
		
		ImGui::Text("Wall Tiles : ");
		for (auto i = WFC_MG2D_TILE_N; i <= WFC_MG2D_TILE_TW; i++)
		{
			ImGui::Image(wallTiles[i].first.GetTexID(), ImVec2(50, 50));
			sprintf(buff, "##PopUpForWallTileSetting_%d", i);
			if (ImGui::BeginPopupContextItem(buff))
			{
				for (auto j = 0; j < wallTiles[i].second.size(); j++)
				{
					ImGui::Image(wallTiles[i].second[j].GetTexID(), ImVec2(50, 50));
					sprintf(buff, "##PopUpForWallTileSettingSub_%d_%d", i, j);
					if (ImGui::BeginPopupContextItem(buff))
					{
						if(ImGui::Button("Rotate")) wallTiles[i].second[j].Rotate();
						if(ImGui::Button("Delete"))
						{
							wallTiles[i].second.erase(wallTiles[i].second.begin() + j);
							ImGui::EndPopup();
							break;
						}
						ImGui::EndPopup();
					}
					
					if ((j + 1) % 3 != 0) ImGui::SameLine();
				}
				ImGui::NewLine();
				if (ImGui::Button("Add"))
				{
					std::string path = Utils::ShowFileOpenDialog();
					if (path.size() > 3)
					{
						int w = 0, h = 0, ch = 4;
						stbi_set_flip_vertically_on_load(false);
						auto d = stbi_load(path.data(), &w, &h, &ch, 4);
						wallTiles[i].second.emplace_back();
						wallTiles[i].second.back().Setup(tileSize[0], tileSize[1]);
						wallTiles[i].second.back().SetData(w, h, d);
						stbi_image_free(d);
					}
				}
				ImGui::EndPopup();
			}
			if ((i + 1) % 3 != 0) ImGui::SameLine();
		}
		ImGui::NewLine();
		ImGui::EndChild();
	}

	void MazeGen2DManager::SetupDefaultTiles()
	{
		int hHeight = tileSize[1] / 4;
		int vWidth = tileSize[0] / 4;
		int Width = tileSize[0];
		int Height  = tileSize[1];

		unsigned char* hori = new unsigned char[4 * tileSize[0] * hHeight];
		unsigned char* vert = new unsigned char[4 * tileSize[1] * vWidth];
		memset(hori, 255, 4 * tileSize[0] * hHeight);
		memset(vert, 255, 4 * tileSize[1] * vWidth);

		// --#--
		// --#--
		// --#--
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_N].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_N].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height, vert);
		this->wallTiles[WFC_MG2D_TILE_N].second.clear();

		// -----
		// -----
		// #####
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_E].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_E].first.WriteToOffset(0, (Height - hHeight) / 2, Width, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_E].second.clear();

		// --@--
		// --@--
		// --###
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_NE].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_NE].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_NE].first.WriteToOffset(Width / 2, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_NE].second.clear();

		// -----
		// -----
		// --###
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_SE].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_SE].first.WriteToOffset((Width - vWidth) / 2, Height / 2, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_SE].first.WriteToOffset(Width / 2, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_SE].second.clear();

		// -----
		// -----
		// ###--
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_SW].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_SW].first.WriteToOffset(0, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_SW].first.WriteToOffset((Width - vWidth) / 2, Height / 2, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_SW].second.clear();

		// --#--
		// --#--
		// ###--
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_NW].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_NW].first.WriteToOffset(0, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_NW].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_NW].second.clear();

		// --#--
		// --#--
		// #####
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_C].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_C].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height, vert);
		this->wallTiles[WFC_MG2D_TILE_C].first.WriteToOffset(0, (Height - hHeight) / 2, Width, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_C].second.clear();

		// --#--
		// --#--
		// --###
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_CE].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_CE].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height, vert);
		this->wallTiles[WFC_MG2D_TILE_CE].first.WriteToOffset(Width / 2, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_CE].second.clear();

		// --#--
		// --#--
		// ###--
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_CW].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_CW].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height, vert);
		this->wallTiles[WFC_MG2D_TILE_CW].first.WriteToOffset(0, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_CW].second.clear();

		// --#--
		// --#--
		// #####
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_CN].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_CN].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_CN].first.WriteToOffset(0, (Height - hHeight) / 2, Width, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_CN].second.clear();

		// -----
		// -----
		// #####
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_CS].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_CS].first.WriteToOffset((Width - vWidth) / 2, Height / 2, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_CS].first.WriteToOffset(0, (Height - hHeight) / 2, Width, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_CS].second.clear();

		// -----
		// -----
		// --#--
		// --#--
		// --#--
		this->wallTiles[WFC_MG2D_TILE_TS].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_TS].first.WriteToOffset((Width - vWidth) / 2, Height / 2, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_TS].second.clear();

		// --#--
		// --#--
		// --#--
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_TN].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_TN].first.WriteToOffset((Width - vWidth) / 2, 0, vWidth, Height / 2, vert);
		this->wallTiles[WFC_MG2D_TILE_TN].second.clear();

		// -----
		// -----
		// --###
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_TE].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_TE].first.WriteToOffset(Width / 2, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_TE].second.clear();

		// -----
		// -----
		// ###--
		// -----
		// -----
		this->wallTiles[WFC_MG2D_TILE_TW].first.Setup(tileSize[0], tileSize[1]);
		this->wallTiles[WFC_MG2D_TILE_TW].first.WriteToOffset(0, (Height - hHeight) / 2, Width / 2, hHeight, hori);
		this->wallTiles[WFC_MG2D_TILE_TW].second.clear();

		WFC_SAFE_DELETE(hori);
		WFC_SAFE_DELETE(vert);

		pathTiles.first.Setup(tileSize[0], tileSize[1]);
		pathTiles.second.clear();
	}

	void MazeGen2DManager::UpdateViewportTexture()
	{
		MazeGen2DTile* currentTile = nullptr;
		for (int i = 0; i < tileMapSize[1]; i++) for (int j = 0; j < tileMapSize[0]; j++)
		{
			//int tileIndex = i * tileMapSize[0] + j;
			bool isWall = algorithms[selectedAlgorithm]->At(j, i, true);
			uint8_t dat = 0b00000000;
			//dat |= algorithms[selectedAlgorithm]->At(j - 1, i - 1, true) ? 0b00000000 : ((0b10000000) >> 0); // north west
			dat |= algorithms[selectedAlgorithm]->At(j, i - 1, true) == false ? 0b00000000 : ((0b10000000) >> 1); // north
			//dat |= algorithms[selectedAlgorithm]->At(j + 1, i - 1, true) ? 0b00000000 : ((0b10000000) >> 2); // north east
			dat |= algorithms[selectedAlgorithm]->At(j + 1, i, true) == false ? 0b00000000 : ((0b10000000) >> 3); // east
			//dat |= algorithms[selectedAlgorithm]->At(j + 1, i + 1, true) ? 0b00000000 : ((0b10000000) >> 4); // sowth east
			dat |= algorithms[selectedAlgorithm]->At(j, i + 1, true) == false ? 0b00000000 : ((0b10000000) >> 5); // south
			//dat |= algorithms[selectedAlgorithm]->At(j - 1, i + 1, true) ? 0b00000000 : ((0b10000000) >> 6); // south west
			dat |= algorithms[selectedAlgorithm]->At(j - 1, i, true) == false ? 0b00000000 : ((0b10000000) >> 7); // west
			if (!isWall) currentTile = (pathTiles.second.size() > 0) ? &(pathTiles.second[rand() % pathTiles.second.size()]) : &(pathTiles.first);
			else if (dat == 0b01000100) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_N);
			else if (dat == 0b00010001) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_E);
			else if (dat == 0b01010000) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_NE);
			else if (dat == 0b00010100) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_SE);
			else if (dat == 0b00000101) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_SW);
			else if (dat == 0b01000001) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_NW);
			else if (dat == 0b01010101) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_C);
			else if (dat == 0b01010100) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_CE);
			else if (dat == 0b01000101) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_CW);
			else if (dat == 0b01010001) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_CN);
			else if (dat == 0b00010101) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_CS);
			else if (dat == 0b01000000) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_TN);
			else if (dat == 0b00000100) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_TS);
			else if (dat == 0b00000001) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_TW);
			else if (dat == 0b00010000) currentTile = WALL_TILE_RAND(WFC_MG2D_TILE_TE);
			else currentTile = &pathTiles.first;
			currentTile->BakeToTexture(j * tileSize[0], i * tileSize[1], tileSize[0] * tileMapSize[0], tileSize[1] * tileMapSize[1], viewportTile.data);
		}
		viewportTile.Upload();
	}

	void MazeGen2DManager::ExportImage()
	{
		UpdateViewportTexture();
		auto exportPath = Utils::ShowSaveFileDialog();
		if (exportPath.size() <= 3) return;
		if (exportPath.find(".png") == std::string::npos) exportPath += ".png";	
		stbi_flip_vertically_on_write(false);
		auto [w, h] = viewportTile.GetSize();
		WFC_LOG("Writing file %s", exportPath.data());
		stbi_write_png(exportPath.data(), w, h, 4, viewportTile.GetDataPTR(), w * 4);
	}

	void MazeGen2DManager::ExportMask(bool value)
	{
		auto exportPath = Utils::ShowSaveFileDialog();
		if (exportPath.size() <= 3) return;
		if (exportPath.find(".png") == std::string::npos) exportPath += ".png";
		int countX = tileMapSize[0];
		int countY = tileMapSize[1];
		int tileSizeX = tileSize[0];
		int tileSizeY = tileSize[1];
		int w = tileSizeX * countX;
		int h = tileSizeY * countY;
		unsigned char* d = new unsigned char[w * h * 4];
		for (int i = 0; i < countY; i++)
		{
			for (int j = 0; j < countX; j++)
			{
				int tileIndex = i * countY + j;
				int startX = j * tileSizeX;
				int startY = i * tileSizeY;
				for (int a = startY; a < startY + tileSizeY; a++)
				{
					for (int b = startX; b < startX + tileSizeX; b++)
					{
						int dInd = (a * w + b) * 4;
						int tInd = ((a - startY) * tileSizeX + (b - startX)) * 4;
						auto val = algorithms[selectedAlgorithm]->At(j, i) ? 255 : 0;
						val = value ? val : 255 - val;
						d[dInd + 0] = val;
						d[dInd + 1] = val;
						d[dInd + 2] = val;
						d[dInd + 3] = val;
					}
				}

			}
		}
		stbi_flip_vertically_on_write(false);
		WFC_LOG("Writing file %s", exportPath.data());
		stbi_write_png(exportPath.data(), w, h, 4, d, w * 4);
		delete[] d;

	}

	void MazeGen2DManager::ExportText()
	{
		auto exportPath = Utils::ShowSaveFileDialog();
		if (exportPath.size() <= 3) return;
		if (exportPath.find(".txt") == std::string::npos) exportPath += ".txt";

		std::string data;
		data.reserve(tileMapSize[1] * (tileMapSize[0] + 1) + 1);
		for (auto i = 0; i < tileMapSize[1]; i++)
		{
			for (auto j = 0; j < tileMapSize[0]; j++)
				data += algorithms[selectedAlgorithm]->At(j, i, true) ? '#' : ' ';
			data += '\n';
		}
		WFC_LOG("Writing file %s", exportPath.data());
		Utils::WriteFile(exportPath, data);
	}

}