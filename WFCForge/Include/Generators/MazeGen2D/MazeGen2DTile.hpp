#pragma once
#include "Graphics/Texture2D.hpp"

#include "imgui.h"
#include "stb_image.h"
#include "avir.h"

#define WFC_MG2D_TILE_N		0
#define WFC_MG2D_TILE_E		2
#define WFC_MG2D_TILE_NE	1
#define WFC_MG2D_TILE_SE	3
#define WFC_MG2D_TILE_SW	4
#define WFC_MG2D_TILE_NW	5
#define WFC_MG2D_TILE_C		6
#define WFC_MG2D_TILE_CE	7
#define WFC_MG2D_TILE_CW	8
#define WFC_MG2D_TILE_CN	9
#define WFC_MG2D_TILE_CS	10
#define WFC_MG2D_TILE_TN	11
#define WFC_MG2D_TILE_TE	12
#define WFC_MG2D_TILE_TS	13
#define WFC_MG2D_TILE_TW	14

namespace WFCForge
{

	class MazeGen2DTile
	{
	public:
		friend class MazeGen2DManager;

		MazeGen2DTile() = default;
		virtual ~MazeGen2DTile();
		MazeGen2DTile(const MazeGen2DTile& other);

		void Setup(int w, int h);
		void SetData(int w, int h, const unsigned char* d);
		void Destroy();
		void Rotate();
		void BakeToTexture(int offsetX, int offsetY, int w, int h, unsigned char* d);
		void WriteToOffset(int offsetX, int offsetY, int w, int h, unsigned char* d);
		void Upload();

		inline ImTextureID GetTexID() { if (tex.IsLoaded()) return (ImTextureID)(intptr_t)tex.GetHandle(); else return nullptr; }
		inline std::pair<int, int> GetSize() { return { width, height }; }
		inline const unsigned char* GetDataPTR() { return this->data; }
	
	private:
		int width = 0, height = 0;
		unsigned char* data = nullptr;
		Texture2D tex;
	};

}