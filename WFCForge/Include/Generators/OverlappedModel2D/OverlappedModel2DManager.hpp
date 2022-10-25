#pragma once
#include "Graphics/Texture2D.hpp"

namespace WFCForge
{
    class OverlappedModel2DManager
    {
    public:
        OverlappedModel2DManager() = default;
        virtual ~OverlappedModel2DManager() = default;

        void Setup();
        void Destroy();
        void Update();

        void* GetViewportTexture();        

        void ShowSettings();

    private:
        Texture2D viewportTexture;
    };
}