#include "Generators/OverlappedModel2D/OverlappedModel2DManager.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
    void OverlappedModel2DManager::Setup()
    {
        viewportTexture.CreateEmpty(100, 100);
    }

    void OverlappedModel2DManager::Destroy()
    {
        viewportTexture.Destroy();
    }

    void OverlappedModel2DManager::Update()
    {
        
    }

    void* OverlappedModel2DManager::GetViewportTexture()
    {
        return (void*)(intptr_t)viewportTexture.GetHandle();
    }

    void OverlappedModel2DManager::ShowSettings()
    {
        ImGui::Text("To be implemented in V0.2");
    }

}