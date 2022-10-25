#include "Generators/TiledModel2D/TiledModel2DManager.hpp"
#include "WFCForge.hpp"


namespace WFCForge
{

    void TiledModel2DManager::Setup()
    {
        viewportTexture.CreateEmpty(100, 100);
    }

    void TiledModel2DManager::Destroy()
    {
        viewportTexture.Destroy();
    }

    void TiledModel2DManager::Update()
    {
        
    }

    void* TiledModel2DManager::GetViewportTexture()
    {
        return (void*)(intptr_t)viewportTexture.GetHandle();
    }

    void TiledModel2DManager::ShowSettings()
    {
        ImGui::Text("void TiledModel2DManager::ShowSettings()");
    }    

}