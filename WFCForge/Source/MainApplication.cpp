#include "Core/Core.hpp"
#include "MainApplication.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{

	static const char* modeNames[] = {
			"Tiled Model",
			"Overlapped Model",
			"Maze Generator",
			"3D Mode",
			"Voxel Humans",
			"Unknown"
	};


	void MainApplication::OnStart()
	{
		if (Utils::PathExists(Utils::GetExecutableDir() + PATH_SEPERATOR "Style.json"))
		{
			Style s;
			s.LoadFromFile(Utils::GetExecutableDir() + PATH_SEPERATOR "Style.json");
			s.Apply();
		}

		mainWindow.SetTitle("WFCForge - Jaysmito Mukherjee");

		appState.mainViewport.SetAppState(&appState);
		appState.tiledModel2D.Setup(&appState);
		appState.mazeGen2D.Setup(&appState);
		appState.voxoMan3D.Setup(&appState);
		appState.overlappedModel2D.Setup();

		WFC_LOG("OnStart");
	}


	void MainApplication::OnUpdate(float deltaTime)
	{
		appState.deltaTime = deltaTime;
		switch (appState.mode)
		{
		case Mode_OverlappedModel2D: appState.overlappedModel2D.Update(); break;
		case Mode_TiledModel2D: appState.tiledModel2D.Update(); break;
		case Mode_MazeGen2D: appState.mazeGen2D.Update(); break;
		case Mode_VoxoMan3D: appState.voxoMan3D.Update(); break;
		}
	}

	void MainApplication::ShowSettingsFor2D()
	{
		if (ImGui::BeginCombo("2D Generator Mode", modeNames[appState.mode]))
		{
			for (int i = 0; i < Mode_3D; i++)
			{
				bool isSelected = (appState.mode == i);
				if (ImGui::Selectable(modeNames[i], isSelected))
					appState.mode = (Mode)i;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (appState.mode == Mode_TiledModel2D)
		{
			ImGui::PushID("Mode_TiledModel2D");
			appState.tiledModel2D.ShowSettings();
			ImGui::PopID();
		}
		else if (appState.mode == Mode_OverlappedModel2D)
		{
			ImGui::PushID("Mode_OverlappedModel2D");
			appState.overlappedModel2D.ShowSettings();
			ImGui::PopID();
		}
		else if (appState.mode == Mode_MazeGen2D)
		{
			ImGui::PushID("Mode_MazeGen2D");
			appState.mazeGen2D.ShowSettings();
			ImGui::PopID();
		}
	}

	void MainApplication::ShowSettingsFor3D()
	{
		if (ImGui::BeginCombo("3D Generator Mode", modeNames[appState.mode]))
		{
			for (auto i = Mode_3D + 1; i < Mode_Unknown; i++)
			{
				bool isSelected = (appState.mode == i);
				if (ImGui::Selectable(modeNames[i], isSelected))
					appState.mode = (Mode)i;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (appState.mode == Mode_VoxoMan3D)
		{
			ImGui::PushID("Mode_VoxoMan3D");
			appState.voxoMan3D.ShowSettings();
			ImGui::PopID();
		}
	}

	void MainApplication::OnUIShow()
	{
		appState.dockspace.Begin();

		appState.mainViewport.OnShow();

		ImGui::Begin("Controls");
		bool val = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
		val = (ImGui::BeginTabBar("##textureStoreTabBar"));
		ImGui::PopStyleVar();
		if (val)
		{

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("   2D   "));
			ImGui::PopStyleVar();
			if (val)
			{
				ShowSettingsFor2D();
				ImGui::EndTabItem();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("   3D   "));
			ImGui::PopStyleVar();
			if (val)
			{
				ShowSettingsFor3D();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

		appState.dockspace.End();
	}

	void MainApplication::OnEnd()
	{
		appState.tiledModel2D.Destroy();
		appState.overlappedModel2D.Destroy();
		appState.mazeGen2D.Destroy();
		appState.voxoMan3D.Destroy();
		WFC_LOG("OnEnd");
	}

	bool MainApplication::OnClose()
	{
		WFC_LOG("OnClose");
		return true;
	}

}