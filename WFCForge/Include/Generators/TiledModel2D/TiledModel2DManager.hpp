#pragma once

namespace WFCForge
{
    class TiledModel2DManager
    {
    public:
        TiledModel2DManager() = default;
        virtual ~TiledModel2DManager() = default;


        void Setup();
        void Destroy();
        
        void ShowSettings();
    };
}