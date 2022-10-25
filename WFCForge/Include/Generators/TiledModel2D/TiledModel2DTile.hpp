#pragma once
#include "Graphics/Texture2D.hpp"

namespace WFCForge
{
    class TiledModel2DTile
    {
    public:
        TiledModel2DTile(const TiledModel2DTile & other);
        TiledModel2DTile(int tileWidth, int tileHeight);
        virtual ~TiledModel2DTile();
        

        int SetData(int width, int height, unsigned char* data, bool isDataRGB = false);
        void UploadToGPU();
        Texture2D& GetGPUTexture();

        inline void Rotate(int count) { for (auto i = 0; i < count; i++) this->RotateOnce(); }
        inline bool IsUploadedToGPU() { return tex.IsLoaded(); }
        inline unsigned char* GetDataPTR() { return data.data(); }
        inline std::string GetFaceIDStr(int index) { return faceIDStrs[index]; }
        inline int32_t GetFaceIDHash(int index) { return faceIDHashes[index]; }
        inline int32_t* GetFaceIDHashPTR(int index) { return &faceIDHashes[index]; }
        inline std::vector<unsigned char> GetFaceID(int index) { return faceIDs[index]; }
        inline uint32_t GetTileHash() { return tileHash; }

        friend bool operator==(const TiledModel2DTile& lhs, const TiledModel2DTile& rhs);

    private:
        void RotateOnce();
        void CalculateFaceIDs();

    private:
        int height, width;
        Texture2D tex;
        std::vector<unsigned char> data;
        std::array<std::vector<unsigned char>, 4> faceIDs; // top right bottom left
        std::array<std::string, 4> faceIDStrs; // top right bottom left
        std::array<int32_t, 4> faceIDHashes; // top right bottom left
        uint32_t tileHash = 0;
    };
}

