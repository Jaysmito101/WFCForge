#include "Generators/MazeGen2D/MazeGen2DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

#include "Generators/MazeGen2D/MazeGen2DDummy.hpp"

namespace WFCForge
{
	void MazeGen2DManager::Setup(AppState* appState)
	{
		this->appState = appState;
		this->viewportTexture.CreateEmpty(100, 100);
		this->wallTileData = new unsigned char[10 * 10 * 4];
		this->pathTileData = new unsigned char[10 * 10 * 4];
		memset(this->wallTileData, 255, 10 * 10 * 4);
		memset(this->pathTileData, 0, 10 * 10 * 4);
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

			static const char* algorithmNames[] = {
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

			if(!generateAllOn) if (ImGui::Button("Clean")) algorithms[selectedAlgorithm]->Destroy();
			
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

}