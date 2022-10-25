#pragma once
#include "Core/Core.hpp"

struct GLFWwindow;

namespace WFCForge
{

	class ImGuiManager
	{
	public:

		static void InitializeImGui(std::string settingsFile, GLFWwindow* window);
		static void ShutdownImGui();
		static void PrepareFrameImGui();
		static void RenderFrameImGui();
	};

}
