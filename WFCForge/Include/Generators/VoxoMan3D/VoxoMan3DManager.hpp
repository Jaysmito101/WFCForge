#pragma once

#include "Graphics/Framebuffer.hpp"
#include "Graphics/BatchRenderer.hpp"
#include "Graphics/Camera.hpp"

#include "Generators/VoxoMan3D/VoxoMan3DTypes.hpp"

#include "imgui.h"

namespace WFCForge
{
	class AppState;

	class VoxoMan3DManager
	{
	public:
		VoxoMan3DManager() = default;
		virtual ~VoxoMan3DManager() = default;

		void Setup(AppState* appState);
		void Destroy();
		void Update();
		void ShowSettings();

		ImTextureID GetViewportTexture() { return (ImTextureID)(intptr_t)fbo.GetColorTex(); }

	private:
		void GenerateNew();
		void ShowBasicGeneratorSettings();

	private:
		AppState* appState = nullptr;
		Framebuffer fbo;
		BatchRenderer renderer;
		Camera cam;
		glm::vec3 lightPosition = glm::vec3(0.0f, 2.0f, 0.0f);
		std::vector<VoxoMan3DVoxel> voxels;
		VoxoMan3DContext context;
	};

}