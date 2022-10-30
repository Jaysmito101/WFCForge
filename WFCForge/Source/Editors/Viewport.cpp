#include "Core/Core.hpp"
#include "Editors/Viewport.hpp"
#include "Data/AppState.hpp"

#include <imgui.h>

namespace WFCForge
{
	void Viewport::OnShow()
	{
		ImGui::Begin("Viewport");
		ImGui::BeginChild("MainViewport");
		ImVec2 wsize = ImGui::GetWindowSize();

		void* textureID = NULL;
		switch(appState->mode)
		{
			case Mode_VoxoMan3D: textureID = appState->voxoMan3D.GetViewportTexture(); break;
			case Mode_OverlappedModel2D : textureID = appState->overlappedModel2D.GetViewportTexture(); break;
			case Mode_TiledModel2D: textureID = appState->tiledModel2D.GetViewportTexture(); break;
			case Mode_MazeGen2D: textureID = appState->mazeGen2D.GetViewportTexture(); break;
		}
		if(textureID) ImGui::Image((ImTextureID)textureID, wsize);

		if (ImGui::IsItemHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			static bool leftB = false, rightB = false, middleB = false;
			appState->mouseButton.left = io.MouseDown[ImGuiMouseButton_Left];
			appState->mouseButton.middle = io.MouseDown[ImGuiMouseButton_Middle];
			appState->mouseButton.right = io.MouseDown[ImGuiMouseButton_Right];

			float scrollDelta = io.MouseWheel;
			appState->scrollDelta = io.MouseWheel;
			if (fabs(scrollDelta) > 0.5f)
			{

			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
			{

			}
			auto offset = ImGui::GetItemRectMin();
			this->mousePosition.first = (io.MousePos.x - offset.x) / wsize.x;
			this->mousePosition.second = (io.MousePos.y - offset.y)/ wsize.y;
		}
		else
		{
			this->mousePosition.first = -1.0f;
			this->mousePosition.second = -1.0f;
		}
		appState->mousePosition.x = this->mousePosition.first;
		appState->mousePosition.y = this->mousePosition.second;
		appState->aspectRatio = wsize.x / wsize.y;

		ImGui::EndChild();



		ImGui::End();

		float aspectRatio = wsize.x / wsize.y;

	}
}