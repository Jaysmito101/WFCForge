#pragma once

#include "Data/AppState.hpp"
#include "Generators/VoxoMan3D/VoxoMan3DHair.hpp"

namespace WFCForge
{
    void VoxoMan3DHair::Setup(AppState* appState)
    {
        this->appState = appState;
        
    }

    void VoxoMan3DHair::Destroy()
    {

    }

    void VoxoMan3DHair::ShowSettings()
    {
        ImGui::Text("Hair Generator!");
    }

    void VoxoMan3DHair::Generate(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
		voxels.push_back({ context.head.start + glm::vec3(0.0f, 0.1f, 0.0f), context.head.size * 1.1f, glm::vec3(0.0f) });
    }
}