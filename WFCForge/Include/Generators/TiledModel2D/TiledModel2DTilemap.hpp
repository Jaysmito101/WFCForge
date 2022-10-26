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
        void Prepare(int countX, int countY, int tileSizeX, int tileSizeY, TiledModel2DTileset* tileset, bool wrapOutput = false);
        void Collapse(int x, int y, int id);
        void CollapseNext();
        void CollapseAll(int seed);
        void ExportImage(std::string path);

        inline bool HasTotallyCollapsed() {return hasTotallyCollapsed;}
        inline bool IsPrepared() { return this->isPrepared; }

    private:
        void PropagateFor(int x, int y);
        void ReduceEntropy(int x, int y, int currentHash, int targetSide);

    public:
        bool wrapOutput = false;
        std::vector<std::vector<int>> tiles;

    private:
        bool hasTotallyCollapsed = false;
        int countX = 0;
        int countY = 0;
        int tileSizeX = 0;
        int tileSizeY = 0;
        bool isPrepared = false;
        std::vector<unsigned char> blankTex;
        std::vector<int> items;
        TiledModel2DTileset tileset;
    };
}