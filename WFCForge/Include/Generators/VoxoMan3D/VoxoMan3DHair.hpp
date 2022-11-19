#pragma once

#include "Core/Core.hpp"
#include "Generators/VoxoMan3D/VoxoMan3DTypes.hpp"

namespace WFCForge
{
	class AppState;

    enum HairStyle
    {
        HairStyle_Simple1,
        HairStyle_Simple2,
        HairStyle_TwoBuns,
        HairStyle_PonyTail,
        HairStyle_Rod,
        HairStyle_Bald,
        HairStyle_SemiBald,
        HairStyle_Count
    };

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
        void GenerateSimple1(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);
        void GenerateSimple2(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);
        void GenerateTwoBuns(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);
        void GeneratePonyTail(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);
        void GenerateRod(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);
        void GenerateSemiBald(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context);

    private:
        AppState* appState = nullptr;
        HairStyle hairStyle = HairStyle_Simple1;
        bool randomizeHairStyle = true;
        float lengthMin = 1.5f;
        float lengthMax = 4.5f;
    };
}