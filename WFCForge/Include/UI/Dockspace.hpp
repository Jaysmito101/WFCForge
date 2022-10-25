#pragma once
#include "Core/Core.hpp"

#include <imgui.h>

namespace WFCForge
{
	class Dockspace
	{
	public:
		Dockspace() = default;
		~Dockspace() = default;

		void Begin();
		void End();

		inline void SetOpen(bool open) { this->isOpen = open; }
		inline bool IsOpen() { return this->isOpen; }

	private:
		bool isOpen = true;
		bool optFullScreenPersistant = true;
		bool optFullScreen = optFullScreenPersistant;
		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	};

}
