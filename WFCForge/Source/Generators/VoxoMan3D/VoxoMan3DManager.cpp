#include "Generators/VoxoMan3D/VoxoMan3DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

namespace WFCForge
{
	void VoxoMan3DManager::Setup(AppState* appState)
	{
		this->appState = appState;
		this->fbo.Setup(512, 512);
		this->renderer.Setup();
		this->cam.GetPositionPTR()[2] = -2.5f;
	}
	
	void VoxoMan3DManager::Destroy()
	{
		this->fbo.Destroy();
		this->renderer.Destroy();
	}
	
	void VoxoMan3DManager::Update()
	{
		this->cam.SetPrespective(appState->aspectRatio);
		this->cam.RecalculateMatrices();

		this->fbo.Begin();
		this->fbo.Clear();
		this->renderer.Begin(this->cam.GetPV(), this->lightPosition, this->cam.GetPosition());
		for (const auto& voxel : this->voxels) this->renderer.Cube(voxel.position, voxel.size, voxel.color);
		this->renderer.End();
		this->fbo.End();

		static float mousePosPrev[] = { 0.0f, 0.0f };
		float mouseDelta[] = { (appState->mousePosition.x - mousePosPrev[0]), (appState->mousePosition.y - mousePosPrev[1]) };
		mousePosPrev[0] = appState->mousePosition.x;
		mousePosPrev[1] = appState->mousePosition.y;
		mouseDelta[0] = std::clamp(mouseDelta[0], -0.8f, 0.8f);
		mouseDelta[1] = std::clamp(mouseDelta[1], -0.8f, 0.8f);
		if (appState->mouseButton.left)
		{
			this->cam.GetPositionPTR()[0] += mouseDelta[0]; 
			this->cam.GetPositionPTR()[1] -= mouseDelta[1];
		}
		if (appState->mouseButton.middle)
		{
			this->cam.GetRotationPTR()[1] += mouseDelta[0];
			this->cam.GetRotationPTR()[0] += mouseDelta[1];
		}
		this->cam.GetPositionPTR()[2] += appState->scrollDelta * 0.1f;
	}



	void VoxoMan3DManager::ShowSettings()
	{
		if (ImGui::CollapsingHeader("Camera Settings"))
		{
			ImGui::PushID("Camera Settings");
			ImGui::DragFloat3("Position", this->cam.GetPositionPTR(), 0.01f);
			ImGui::DragFloat3("Rotation", this->cam.GetRotationPTR(), 0.01f);
			ImGui::PopID();
		}
		if (ImGui::CollapsingHeader("Light Settings"))
		{
			ImGui::PushID("Light Settings");
			ImGui::DragFloat3("Position", glm::value_ptr(this->lightPosition), 0.01f);
			ImGui::PopID();
		}

		if (ImGui::CollapsingHeader("Generator Settings"))
		{
			ImGui::PushID("Generator Settings");
			
			if (ImGui::Button("Generate")) this->GenerateNew();

			ImGui::PopID();
		}

	}
	
	void VoxoMan3DManager::GenerateNew()
	{
		voxels.clear();
		voxels.push_back({ { 0, 1, 0 }, { 1, 1, 1 }, { 0.5, 0.1, 0.8 } });
		auto num = 1000;
		for (auto i = 0; i < num; i++) voxels.push_back({ { Utils::Random(-0.5, 0.5), Utils::Random(), Utils::Random(-0.5f, 0.5f)}, {Utils::Random(0.2, 0.5), Utils::Random(0.2, 0.5), Utils::Random(0.2, 0.5)}, {Utils::Random(), Utils::Random(), Utils::Random()}});
	}
}