#pragma once

#include "Graphics/Texture2D.hpp"
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"
#include "Generators/MazeGen2D/MazeGen2DTile.hpp"

#include <imgui.h>

namespace WFCForge
{
	class AppState;

	class MazeGen2DManager
	{
	public:
		MazeGen2DManager() = default;
		~MazeGen2DManager() = default;

		void Setup(AppState* appState);
		void Destroy();
		void Update();
		void ShowSettings();

		inline ImTextureID GetViewportTexture() { return viewportTile.GetTexID(); }

	private:
		void ShowChangeTileSettings();
		void SetupDefaultTiles();
		void UpdateViewportTexture();
		void ExportImage();
		void ExportMask(bool value);


	private:
		int selectedAlgorithm = 0;
		int tileMapSize[2] = {10, 10};
		int tileSize[2] = {10, 10};
		std::array<std::pair<MazeGen2DTile, std::vector<MazeGen2DTile>>, 15> wallTiles;
		std::pair<MazeGen2DTile, std::vector<MazeGen2DTile>> pathTiles;
		MazeGen2DTile viewportTile;
		AppState* appState = nullptr;
		std::vector<std::shared_ptr<MazeGen2DAlgorithm>> algorithms;
	};
}