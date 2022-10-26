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

        void AutoCalculateTileNeighbours();

        void RemoveTile(int hash);
        void AddTile(TiledModel2DTile tile);        

        inline void Clear() { tiles.clear(); hashes.clear(); }
        
    private:        

    public:
        std::vector<int> hashes;
        std::unordered_map<int, TiledModel2DTile> tiles;
    };
}