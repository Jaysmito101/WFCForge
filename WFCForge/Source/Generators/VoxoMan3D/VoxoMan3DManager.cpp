#include "Generators/VoxoMan3D/VoxoMan3DManager.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

#include <glad/glad.h>

namespace WFCForge
{
	void VoxoMan3DManager::Setup(AppState* appState)
	{
		this->appState = appState;
		this->fbo.Setup(512, 512);
		this->renderer.Setup();
		this->cam.GetPositionPTR()[2] = -2.5f;
		this->cam.GetRotationPTR()[2] = 3.14f;
		this->cam.GetRotationPTR()[1] = -3.14f;
		this->hairGen.Setup(appState);
	}
	
	void VoxoMan3DManager::Destroy()
	{
		this->hairGen.Destroy();
		this->fbo.Destroy();
		this->renderer.Destroy();
	}
	
	void VoxoMan3DManager::Update()
	{
		this->cam.SetPrespective(appState->aspectRatio);
		this->cam.RecalculateMatrices();

		this->fbo.Begin();
		this->fbo.Clear();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

	void VoxoMan3DManager::ShowBasicGeneratorSettings()
	{
		ImGui::DragInt("Global Seed", &context.seed);
		ImGui::Checkbox("Is Male", &context.isMale);
		ImGui::DragFloat3("Head Size", glm::value_ptr(context.head.prefferedSize), 0.01f, 0.001f);
		ImGui::DragFloat3("Head Size Limits", glm::value_ptr(context.head.prefferedSizeLimits), 0.01f, 0.001f);
		ImGui::NewLine();
		ImGui::DragFloat("Neck Height", &context.neck.prefferredHeight, 0.01f);
		ImGui::DragFloat("Neck Height Limits", &context.neck.heightLimits, 0.001f);
		ImGui::DragFloat("Neck XY Size Factor", &context.neck.prefferedXYSizeFactor, 0.01f);
		ImGui::DragFloat("Neck XY Size Factor Limits", &context.neck.xySizeFactorLimits, 0.01f);
		ImGui::NewLine();
		ImGui::DragFloat3("Torso Size", glm::value_ptr(context.torso.prefferredSize), 0.01f);
		ImGui::DragFloat3("Torso Size Limits", glm::value_ptr(context.torso.prefferredSizeLimits), 0.01f);
		if (!context.isMale)
		{
			ImGui::DragFloat3("Breast Size", glm::value_ptr(context.torso.breastSizePreffered), 0.01f);
			ImGui::DragFloat3("Breast Size Limits", glm::value_ptr(context.torso.breastSizeLimits), 0.01f);
		}
		ImGui::NewLine();
		ImGui::DragFloat3("Arms Size", glm::value_ptr(context.arms.prefferredSize), 0.01f);
		ImGui::DragFloat3("Arms Size Limits", glm::value_ptr(context.arms.prefferredSizeLimits), 0.001f);
		ImGui::NewLine();
		ImGui::DragFloat3("Legs Size", glm::value_ptr(context.legs.prefferredSize), 0.01f);
		ImGui::DragFloat3("Legs Size Limits", glm::value_ptr(context.legs.prefferredSizeLimits), 0.001f);
		ImGui::NewLine();
		ImGui::DragFloat3("Feet Size", glm::value_ptr(context.feet.prefferredSize), 0.01f);
		ImGui::DragFloat3("Feet Size Limits", glm::value_ptr(context.feet.prefferredSizeLimits), 0.001f);
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Generated Info"))
		{
			ImGui::Text("head.size : (%f %f %f)", context.head.size.x, context.head.size.y, context.head.size.z);
			ImGui::Text("head.start: (%f %f %f)", context.head.start.x, context.head.start.y, context.head.start.z);
			ImGui::Text("head.end: (%f %f %f)", context.head.end.x, context.head.end.y, context.head.end.z);
			ImGui::NewLine();
			ImGui::Text("neck.start: (%f %f %f)", context.neck.start.x, context.neck.start.y, context.neck.start.z);
			ImGui::Text("neck.end: (%f %f %f)", context.neck.end.x, context.neck.end.y, context.neck.end.z);
			ImGui::Text("neck.size: (%f %f %f)", context.neck.size.x, context.neck.size.y, context.neck.size.z);
			ImGui::Text("neck.height : (%f)", context.neck.height);
			ImGui::Text("neck.xySizeFactor : (%f)", context.neck.xySizeFactor);
			ImGui::NewLine();
			ImGui::Text("torso.start: (%f %f %f)", context.torso.start.x, context.torso.start.y, context.torso.start.z);
			ImGui::Text("torso.end : (%f %f %f)", context.torso.end.x, context.torso.end.y, context.torso.end.z);
			ImGui::Text("torso.size : (%f %f %f)", context.torso.size.x, context.torso.size.y, context.torso.size.z);
			if (!context.isMale)
			{
				ImGui::Text("torso.breastSize : (%f %f %f)", context.torso.breastSize.x, context.torso.breastSize.y, context.torso.breastSize.z);
				ImGui::Text("torso.breastLeftStart: (%f %f %f)", context.torso.breastLeftStart.x, context.torso.breastLeftStart.y, context.torso.breastLeftStart.z);
				ImGui::Text("torso.breastRightStart: (%f %f %f)", context.torso.breastRightStart.x, context.torso.breastRightStart.y, context.torso.breastRightStart.z);
			}
			ImGui::NewLine();
			ImGui::Text("arms.startR: (%f %f %f)", context.arms.startR.x, context.arms.startR.y, context.arms.startR.z);
			ImGui::Text("arms.endR : (%f %f %f)", context.arms.endR.x, context.arms.endR.y, context.arms.endR.z);
			ImGui::Text("arms.startL: (%f %f %f)", context.arms.startL.x, context.arms.startL.y, context.arms.startL.z);
			ImGui::Text("arms.endL : (%f %f %f)", context.arms.endL.x, context.arms.endL.y, context.arms.endL.z);
			ImGui::Text("arms.size : (%f %f %f)", context.arms.size.x, context.arms.size.y, context.arms.size.z);
			ImGui::NewLine();
			ImGui::Text("legs.startR: (%f %f %f)", context.legs.startR.x, context.legs.startR.y, context.legs.startR.z);
			ImGui::Text("legs.endR : (%f %f %f)", context.legs.endR.x, context.legs.endR.y, context.legs.endR.z);
			ImGui::Text("legs.startL: (%f %f %f)", context.legs.startL.x, context.legs.startL.y, context.legs.startL.z);
			ImGui::Text("legs.endL : (%f %f %f)", context.legs.endL.x, context.legs.endL.y, context.legs.endL.z);
			ImGui::Text("legs.size : (%f %f %f)", context.legs.size.x, context.legs.size.y, context.legs.size.z);
			ImGui::NewLine();
			ImGui::Text("feet.startR: (%f %f %f)", context.feet.startR.x, context.feet.startR.y, context.feet.startR.z);
			ImGui::Text("feet.endR : (%f %f %f)", context.feet.endR.x, context.feet.endR.y, context.feet.endR.z);
			ImGui::Text("feet.startL: (%f %f %f)", context.feet.startL.x, context.feet.startL.y, context.feet.startL.z);
			ImGui::Text("feet.endL : (%f %f %f)", context.feet.endL.x, context.feet.endL.y, context.feet.endL.z);
			ImGui::Text("feet.size : (%f %f %f)", context.feet.size.x, context.feet.size.y, context.feet.size.z);
			ImGui::NewLine();
		}
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
			
			if (ImGui::CollapsingHeader("Basic Generator Settings"))
			{
				ImGui::PushID("Basic Generator Settings");
				ShowBasicGeneratorSettings();
				ImGui::PopID();
			}

			if(ImGui::CollapsingHeader("Hair Generator Settings"))
			{
				ImGui::PushID("Hair Generator Settings");
				this->hairGen.ShowSettings();
				ImGui::PopID();
			}

			if (ImGui::Button("Generate")) this->GenerateNew();
			ImGui::PopID();
		}

	}
	
	void VoxoMan3DManager::GenerateNew()
	{
		voxels.clear();
		
		// generate basic shape

		// srand(context.seed);
		// first randomize the size info
		context.head.size = Utils::Random(context.head.prefferedSize, context.head.prefferedSizeLimits);
		context.neck.height = context.neck.prefferredHeight + Utils::Random(-context.neck.heightLimits, context.neck.heightLimits);
		context.neck.xySizeFactor = context.neck.prefferedXYSizeFactor + Utils::Random(-context.neck.xySizeFactorLimits, context.neck.xySizeFactorLimits);
		context.neck.size = context.head.size * context.neck.xySizeFactor;
		context.neck.size.y = context.neck.height;
		context.torso.size = Utils::Random(context.torso.prefferredSize, context.torso.prefferredSizeLimits);
		context.arms.size = Utils::Random(context.arms.prefferredSize, context.arms.prefferredSizeLimits);
		context.legs.size = Utils::Random(context.legs.prefferredSize, context.legs.prefferredSizeLimits);
		context.feet.size = Utils::Random(context.feet.prefferredSize, context.feet.prefferredSizeLimits);
		context.legs.size = Utils::Random(context.legs.prefferredSize, context.legs.prefferredSizeLimits);
		context.torso.breastSize = Utils::Random(context.torso.breastSizePreffered, context.torso.breastSizeLimits);

		// generate heights
		context.feet.startL.y = context.feet.startR.y = 0.0f;
		context.legs.startL.y = context.legs.startR.y = context.feet.size.y;
		context.torso.start.y = context.legs.startL.y + context.legs.size.y;
		context.neck.start.y = context.torso.start.y + context.torso.size.y;
		context.head.start.y = context.neck.start.y + context.neck.height;

		//  generate xz coords
		context.head.start.x = -context.head.size.x / 2;
		context.head.start.z = -context.head.size.z / 2;
		context.head.end = context.head.start + context.head.size;

		context.neck.start.x = -context.neck.size.x / 2;;
		context.neck.start.z = -context.neck.size.z / 2;;
		context.neck.end = context.neck.start + context.neck.size;

		context.torso.start.x = -context.torso.size.x / 2;;
		context.torso.start.z = -context.torso.size.z / 2;;
		context.torso.end = context.torso.start + context.torso.size;

		context.arms.startL.y = context.arms.startR.y = context.torso.end.y - context.arms.size.y;
		context.arms.startL.z = -context.arms.size.z / 2;
		context.arms.startL.x = -(context.arms.size.x * 2.0f + context.torso.size.x) / 2 - 0.02f;
		context.arms.startR.z = -context.arms.size.z / 2;
		context.arms.startR.x = (context.torso.size.x) / 2 + 0.02f;
		context.arms.endL = context.arms.startL + context.arms.size;
		context.arms.endR = context.arms.startR + context.arms.size;

		context.legs.startL.z = -context.legs.size.z / 2;
		context.legs.startL.x = -context.torso.size.x / 2;
		context.legs.startR.z = -context.legs.size.z / 2;
		context.legs.startR.x = context.torso.size.x / 2 - context.legs.size.x;
		context.legs.endL = context.legs.startL + context.legs.size;
		context.legs.endR = context.legs.startR + context.legs.size;

		context.feet.size.x = context.legs.size.x;
		context.feet.startL.x = context.legs.startL.x;
		context.feet.startL.z = context.legs.startL.z;
		context.feet.startR.x = context.legs.startR.x;
		context.feet.startR.z = context.legs.startR.z;
		context.feet.endL = context.feet.startL + context.feet.size;
		context.feet.endR = context.feet.startR + context.feet.size;

		context.torso.breastSize.x = context.torso.size.x * 0.485f;
		context.torso.breastLeftStart.y = context.torso.end.y - context.torso.breastSize.y - context.torso.size.y * 0.05f;
		context.torso.breastLeftStart.x = context.torso.start.x;
		context.torso.breastLeftStart.z = context.torso.end.z;
		context.torso.breastRightStart.y = context.torso.end.y - context.torso.breastSize.y - context.torso.size.y * 0.05f;
		context.torso.breastRightStart.x = context.torso.end.x - context.torso.breastSize.x;
		context.torso.breastRightStart.z = context.torso.end.z;

		context.skinColor = Utils::Random(context.skinColorPalatte);

		// add the basic voxels
		voxels.push_back({ context.head.start, context.head.size, context.skinColor });
		voxels.push_back({ context.neck.start, context.neck.size, context.skinColor });
		voxels.push_back({ context.torso.start, context.torso.size, context.skinColor });

		voxels.push_back({ context.arms.startL, context.arms.size, context.skinColor });
		voxels.push_back({ context.arms.startR, context.arms.size, context.skinColor });
		
		voxels.push_back({ context.legs.startL, context.legs.size, context.skinColor });
		voxels.push_back({ context.legs.startR, context.legs.size, context.skinColor });

		voxels.push_back({ context.feet.startR, context.feet.size, context.skinColor });
		voxels.push_back({ context.feet.startL, context.feet.size, context.skinColor });

		if (!context.isMale)
		{
			voxels.push_back({ context.torso.breastLeftStart, context.torso.breastSize, context.skinColor });
			voxels.push_back({ context.torso.breastRightStart, context.torso.breastSize, context.skinColor });

		}

		// add the hair
		this->hairGen.Generate(voxels, context);
	}
}