#pragma once
#include "Graphics/Texture2D.hpp"
#include "Generators/TiledModel2D/TiledModel2DTileset.hpp"

namespace WFCForge
{
    class TiledModel2DTilemap
    {
    public:
        TiledModel2DTilemap();
        virtual ~TiledModel2DTilemap();

        void Clean();
        void BakeToTexture(Texture2D* tex);
        void Prepare(int countX, int countY, int tileSizeX, int tileSizeY, TiledModel2DTileset* tileset);

        inline bool IsPrepared() { return this->isPrepared; }

    private:

    private:
        std::vector<TiledModel2DTileset> tiles;
        int countX = 0;
        int countY = 0;
        int tileSizeX = 0;
        int tileSizeY = 0;
        bool isPrepared = false;
    };
}