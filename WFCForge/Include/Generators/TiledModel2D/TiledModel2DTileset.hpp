#pragma once
#include "Generators/TiledModel2D/TiledModel2DTile.hpp"
#include "Graphics/Texture2D.hpp"

namespace WFCForge
{
    class TiledModel2DTileset
    {
    public:
        TiledModel2DTileset();
        virtual ~TiledModel2DTileset();

        void Clone(TiledModel2DTileset* other);

        void AddTile(TiledModel2DTile tile);
        void UploadTilesToGPU();
        void Collapse(int index);

        inline void Clear() { tiles.clear(); }

        inline TiledModel2DTile* Back() { return &tiles.back(); }
        
    private:
        

    public:
        std::vector<TiledModel2DTile> tiles;
    };
}