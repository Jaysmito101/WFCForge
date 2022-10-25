#include "Core/Core.hpp"
#include "MainApplication.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{	

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
		appState.tiledModel2D.Setup();
		appState.overlappedModel2D.Setup();

		WFC_LOG("OnStart");
	}


	void MainApplication::OnUpdate(float deltaTime)
	{
		appState.deltaTime = deltaTime;
		switch(appState.mode)
		{
			case Mode_OverlappedModel2D : appState.overlappedModel2D.Update(); break;
			case Mode_TiledModel2D : appState.tiledModel2D.Update(); break;
		}
	}

	void MainApplication::ShowSettingsFor2D()
	{
		bool val = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
		val = (ImGui::BeginTabBar("##textureStoreTabBar"));
		ImGui::PopStyleVar();
		if(val)
		{			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("Tiled Model"));
			ImGui::PopStyleVar();
			if(val)
			{
				appState.mode = Mode_TiledModel2D;
				ImGui::PushID("Mode_TiledModel2D");
				appState.tiledModel2D.ShowSettings();
				ImGui::PopID();
				ImGui::EndTabItem();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("Overlapped Model"));
			ImGui::PopStyleVar();
			if(val)
			{
				appState.mode = Mode_OverlappedModel2D;
				ImGui::PushID("Mode_OverlappedModel2D");
				appState.overlappedModel2D.ShowSettings();
				ImGui::PopID();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	void MainApplication::ShowSettingsFor3D()
	{
		appState.mode = Mode_Unknown;
		ImGui::Text("Yet to be implemented");
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
		if(val)
		{
			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("   2D   "));
			ImGui::PopStyleVar();
			if(val)
			{
				ShowSettingsFor2D();
				ImGui::EndTabItem();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
			val = (ImGui::BeginTabItem("   3D   "));
			ImGui::PopStyleVar();
			if(val)
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
		WFC_LOG("OnEnd");
	}

	bool MainApplication::OnClose()
	{
		WFC_LOG("OnClose");
		return true;
	}

}