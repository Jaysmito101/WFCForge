#pragma once

#include "Data/AppState.hpp"
#include "Generators/VoxoMan3D/VoxoMan3DHair.hpp"

static const std::vector<glm::vec3> hairColors = {
            WFC_RGB(36,28,17),
            WFC_RGB(50,40,29),
            WFC_RGB(64,52,40),
            WFC_RGB(78,64,51),
            WFC_RGB(35, 18, 11)
        };

namespace WFCForge
{

    void VoxoMan3DHair::GenerateSimple1(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        float temp = 0.0f;
        const glm::vec3 hairColor = Utils::Random(hairColors);
        const float hairThickness = Utils::Random(0.02f, 0.05f);
        glm::vec3 hairTopStart = context.head.start;
        hairTopStart.y += context.head.size.y;
        float hairTopSizeXYFactor = 1.0f;
        glm::vec3 hairTopSize = context.head.size * hairTopSizeXYFactor;
        hairTopSize.y = hairThickness;
        temp = hairTopStart.x;
        hairTopStart.x += hairTopSize.x * 0.5f * (1.0f - hairTopSizeXYFactor);
        hairTopStart.z += hairTopSize.z * 0.5f * (1.0f - hairTopSizeXYFactor);
		voxels.push_back({ hairTopStart, hairTopSize, hairColor }); // hair top
        float sidesFactor = !context.isMale ? Utils::Random(1.7f, 2.5f) : Utils::Random(0.7f, 1.0f);
        voxels.push_back({ glm::vec3(temp - hairThickness, context.head.start.y + context.head.size.y * (1.0f - sidesFactor), hairTopStart.z), glm::vec3(hairThickness, context.head.size.y * sidesFactor, hairTopSize.z), hairColor});
        voxels.push_back({ glm::vec3(context.head.end.x, context.head.start.y + context.head.size.y * (1.0f - sidesFactor), hairTopStart.z), glm::vec3(hairThickness,  context.head.size.y * sidesFactor, hairTopSize.z), hairColor});
        auto hairBackSize = context.head.size;
        float height = hairBackSize.y * (context.isMale ? Utils::Random(0.8f, 1.0f) : Utils::Random(lengthMin, lengthMax));
        hairBackSize.y = height;
        hairBackSize.z = hairThickness;
        voxels.push_back({ context.head.start + glm::vec3(0.0f, -(height - context.head.size.y), -hairThickness), hairBackSize, hairColor});
        
    }

    void VoxoMan3DHair::GenerateSimple2(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        GenerateSimple1(voxels, context);
        
    }

    void VoxoMan3DHair::GenerateTwoBuns(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        GenerateSimple1(voxels, context);
    }

    void VoxoMan3DHair::GeneratePonyTail(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        GenerateSimple1(voxels, context);
    }

    void VoxoMan3DHair::GenerateRod(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        const glm::vec3 hairColor = Utils::Random(hairColors);
        const float hairThickness = Utils::Random(0.02f, 0.05f);
        const float scale = Utils::Random(1.2f, 2.0f);
        glm::vec3 hairSize = context.head.size * scale;
        hairSize.x = hairThickness;
        glm::vec3 hairStart = context.head.start;
        hairStart.x += context.head.size.x / 2.0f - hairThickness / 2.0f;
        hairStart.z -= (hairSize.z - context.head.size.z) + 0.02f;
        hairStart.y += 0.02f;
        voxels.push_back({ hairStart, hairSize, hairColor });
        if(rand() % 10 > 6)
        {
            glm::vec3 hairSize2 = hairSize * 0.7f;
            voxels.push_back({ hairStart + glm::vec3(hairThickness * 1.3f, hairSize.y * 0.3, 0.0f), hairSize2, hairColor });
            voxels.push_back({ hairStart + glm::vec3(-hairThickness * 1.3f, hairSize.y * 0.3, 0.0f), hairSize2, hairColor });
        }
    }

    void VoxoMan3DHair::GenerateSemiBald(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {        
        float temp = 0.0f;
        const glm::vec3 hairColor = Utils::Random(hairColors);
        const float hairThickness = Utils::Random(0.02f, 0.05f);
        glm::vec3 hairTopStart = context.head.start;
        hairTopStart.y += context.head.size.y;
        float hairTopSizeXYFactor = 1.0f;
        glm::vec3 hairTopSize = context.head.size * hairTopSizeXYFactor;
        hairTopSize.y = hairThickness;
        temp = hairTopStart.x;
        hairTopStart.x += hairTopSize.x * 0.5f * (1.0f - hairTopSizeXYFactor);
        hairTopStart.z += hairTopSize.z * 0.5f * (1.0f - hairTopSizeXYFactor);
        float sidesFactor = Utils::Random(0.7f, 1.0f);
        voxels.push_back({ glm::vec3(temp - hairThickness, context.head.start.y + context.head.size.y * (1.0f - sidesFactor), hairTopStart.z), glm::vec3(hairThickness, context.head.size.y * sidesFactor, hairTopSize.z), hairColor});
        voxels.push_back({ glm::vec3(context.head.end.x, context.head.start.y + context.head.size.y * (1.0f - sidesFactor), hairTopStart.z), glm::vec3(hairThickness,  context.head.size.y * sidesFactor, hairTopSize.z), hairColor});
        auto hairBackSize = context.head.size;
        float height = hairBackSize.y * Utils::Random(0.7f, 1.0f);
        hairBackSize.y = height;
        hairBackSize.z = hairThickness;
        voxels.push_back({ context.head.start + glm::vec3(0.0f, -(height - context.head.size.y), -hairThickness), hairBackSize, hairColor});        
    }


    void VoxoMan3DHair::Setup(AppState* appState)
    {
        this->appState = appState;
        
    }

    void VoxoMan3DHair::Destroy()
    {

    }

    void VoxoMan3DHair::ShowSettings()
    {
        static const char* hairStyleNames[] = {
            "Simple 1",
            "Simple 2",
            "Two Buns",
            "Pony Tail",
            "Rod",
            "Bald",
            "Semi Bald"
        };

        // imgui combox for hairStyles
        if (ImGui::BeginCombo("Hair Style", hairStyleNames[hairStyle]))
		{
			for (auto i = 0; i < HairStyle_Count; i++)
			{
				bool isSelected = (hairStyle == i);
				if (ImGui::Selectable(hairStyleNames[i], isSelected))
					hairStyle = (HairStyle)i;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
        ImGui::Checkbox("Randomize Hiar Style", &this->randomizeHairStyle);
        ImGui::DragFloat("Hair Length Min", &lengthMin, 0.01f, 0.01f);
        ImGui::DragFloat("Hair Length Max", &lengthMax, 0.01f, 0.01f);
    }

    void VoxoMan3DHair::Generate(std::vector<VoxoMan3DVoxel>& voxels, VoxoMan3DContext& context)
    {
        if(randomizeHairStyle) hairStyle = (HairStyle)(rand() % HairStyle_Count);
        switch(hairStyle)
        {
            case HairStyle_Simple1: GenerateSimple1(voxels, context); break;
            case HairStyle_Simple2: GenerateSimple2(voxels, context); break;
            case HairStyle_TwoBuns: GenerateTwoBuns(voxels, context); break;
            case HairStyle_PonyTail: GeneratePonyTail(voxels, context); break;
            case HairStyle_Rod: GenerateRod(voxels, context); break;
            case HairStyle_SemiBald: GenerateSemiBald(voxels, context); break;
        }
    }

}