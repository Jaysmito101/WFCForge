#pragma once

#include "Graphics/Texture2D.hpp"
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"

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

		inline ImTextureID GetViewportTexture() { return (ImTextureID)(intptr_t)viewportTexture.GetHandle(); }

	private:
		void UpdateViewportTexture();
		void ExportImage();
		void ExportMask(bool value);

	private:
		int selectedAlgorithm = 0;
		int tileMapSize[2] = {10, 10};
		int tileSize[2] = {10, 10};
		unsigned char* wallTileData = nullptr;
		unsigned char* pathTileData = nullptr;
		Texture2D viewportTexture;
		AppState* appState = nullptr;
		std::vector<std::shared_ptr<MazeGen2DAlgorithm>> algorithms;
	};
}