#pragma once
#include "Graphics/Texture2D.hpp"

#define WFC_TM2D_TILE_TOP      0
#define WFC_TM2D_TILE_RIGHT    1 
#define WFC_TM2D_TILE_BOTTOM   2
#define WFC_TM2D_TILE_LEFT     3

namespace WFCForge
{
    class TiledModel2DTileset;

    class TiledModel2DTile
    {
    public:

        friend class TiledModel2DTileset;

        TiledModel2DTile() = default;
        TiledModel2DTile(const TiledModel2DTile & other);
        TiledModel2DTile(int tileWidth, int tileHeight);
        virtual ~TiledModel2DTile();
        

        int SetData(int width, int height, unsigned char* data, bool isDataRGB = false);
        void UploadToGPU();
        Texture2D& GetGPUTexture();
        bool IsTileAllowed(int newTileHash, int position);

        inline void Rotate(int count) { for (auto i = 0; i < count; i++) this->RotateOnce(); }
        inline bool IsUploadedToGPU() { return tex.IsLoaded(); }
        inline unsigned char* GetDataPTR() { return data.data(); }
        inline int32_t GetFaceIDHash(int index) { return faceIDHashes[index]; }
        inline int32_t* GetFaceIDHashPTR(int index) { return &faceIDHashes[index]; }
        inline std::vector<unsigned char> GetFaceID(int index) { return faceIDs[index]; }
        inline uint32_t GetTileHash() { return tileHash; }

        friend bool operator==(const TiledModel2DTile& lhs, const TiledModel2DTile& rhs);

    private:
        void RotateOnce();
        void CalculateFaceIDs();

    public:
        std::vector<unsigned char> data;
        std::array<std::vector<int>, 4> allowedTiles;

    private:
        int height, width;
        Texture2D tex;
        std::array<std::vector<unsigned char>, 4> faceIDs; // top right bottom left
        std::array<int32_t, 4> faceIDHashes; // top right bottom left
        uint32_t tileHash = 0;
    };
}

