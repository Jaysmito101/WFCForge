#pragma once

#include "Core/Core.hpp"
#include "Generators/VoxoMan3D/VoxoMan3DTypes.hpp"

namespace WFCForge
{
	class AppState;

    class VoxoMan3DHair
    {
    public:
        VoxoMan3DHair() = default;
        virtual ~VoxoMan3DHair() = default;

        void Setup(AppState* appState);
        void Destroy();
        void ShowSettings();
        void Generate(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);

    private:
        AppState* appState;
    };
}