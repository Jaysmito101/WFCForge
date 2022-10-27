#include "Generators/MazeGen2D/MazeGen2DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

#include "Generators/MazeGen2D/MazeGen2DDummy.hpp"
#include "Generators/MazeGen2D/MazeGen2DBinaryTree.hpp"
#include "Generators/MazeGen2D/MazeGen2DAldousBroder.hpp"

#include <stb_image.h>
#include <stb_image_write.h>
#include <avir.h>

namespace WFCForge
{
	static void ChangeTileFor(unsigned char* data, int width, int height)
	{
		std::string path = Utils::ShowFileOpenDialog();
		if (path.size() <= 3) return;
		int w = 0, h = 0, ch = 4;
		stbi_set_flip_vertically_on_load(false);
		auto d = stbi_load(path.data(), &w, &h, &ch, 4);
		if (d == nullptr) return;
		if (w == width && h == height) memcpy(data, d, w * h * 4);
		else
		{
			avir::CImageResizer<> ImageResizer(8);
			ImageResizer.resizeImage(d, w, h, 0, data, width, height, 4, 0);
		}
		stbi_image_free(d);
	}

	void MazeGen2DManager::Setup(AppState* appState)
	{
		this->appState = appState;
		this->viewportTexture.CreateEmpty(100, 100);
		this->wallTileData = new unsigned char[10 * 10 * 4];
		this->pathTileData = new unsigned char[10 * 10 * 4];
		memset(this->wallTileData, 255, 10 * 10 * 4);
		memset(this->pathTileData, 0, 10 * 10 * 4);
		this->algorithms.push_back(std::make_shared<MazeGen2DBinaryTree>());
		this->algorithms.push_back(std::make_shared<MazeGen2DAldousBroder>());
		this->algorithms.push_back(std::make_shared<MazeGen2DDummy>());
	}

	void MazeGen2DManager::Destroy()
	{
		if (this->viewportTexture.IsLoaded()) this->viewportTexture.Destroy();
		WFC_SAFE_DELETE(this->wallTileData);
		WFC_SAFE_DELETE(this->pathTileData);
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
					WFC_SAFE_DELETE(this->wallTileData);
					WFC_SAFE_DELETE(this->pathTileData);
					this->wallTileData = new unsigned char[tileSize[0] * tileSize[1] * 4];
					this->pathTileData = new unsigned char[tileSize[0] * tileSize[1] * 4];
					memset(this->wallTileData, 255, tileSize[0] * tileSize[1] * 4);
					memset(this->pathTileData, 0, tileSize[0] * tileSize[1] * 4);
				}
			}

			ImGui::DragInt2("Tile Map Size", tileMapSize, 0.1f, 1);

			if (ImGui::Button("Change Wall Tile")) ChangeTileFor(wallTileData, tileSize[0], tileSize[1]);
			if (ImGui::Button("Change Path Tile")) ChangeTileFor(pathTileData, tileSize[0], tileSize[1]);

			static const char* algorithmNames[] = {
				"Binary Tree",
				"Aldous Broder",
				"Dummy"
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
				if (viewportTexture.IsLoaded()) viewportTexture.Destroy();
				viewportTexture.CreateEmpty(tileMapSize[0] * tileSize[0], tileMapSize[1] * tileSize[1]);
				algorithms[selectedAlgorithm]->Setup(tileMapSize[0], tileMapSize[1], seed);
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

			if (!generateAllOn)
			{
				if (ImGui::Button("Clean")) algorithms[selectedAlgorithm]->Destroy();
				if (ImGui::Button("Export Image")) ExportImage();
				if (ImGui::Button("Export Path Mask")) ExportMask(false);
				if (ImGui::Button("Export Wall Mask")) ExportMask(true);
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

	void MazeGen2DManager::UpdateViewportTexture()
	{
		for (int i = 0; i < tileMapSize[1]; i++) for (int j = 0; j < tileMapSize[0]; j++)
		{
			auto dat = algorithms[selectedAlgorithm]->At(j, i) ? wallTileData : pathTileData;
			viewportTexture.UploadData(j * tileSize[0], i * tileSize[1], tileSize[0], tileSize[1], dat);
		}
	}
	
	void MazeGen2DManager::ExportImage()
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
						auto dataPTR = algorithms[selectedAlgorithm]->At(j, i) ? wallTileData : pathTileData;
						d[dInd + 0] = dataPTR[tInd + 0];
						d[dInd + 1] = dataPTR[tInd + 1];
						d[dInd + 2] = dataPTR[tInd + 2];
						d[dInd + 3] = dataPTR[tInd + 3];
					}
				}

			}
		}
		stbi_flip_vertically_on_write(false);
		stbi_write_png(exportPath.data(), w, h, 4, d, w * 4);
		delete[] d;
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
		stbi_write_png(exportPath.data(), w, h, 4, d, w * 4);
		delete[] d;

	}

}