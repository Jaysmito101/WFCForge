#pragma once
#include "Generators/TiledModel2D/TiledModel2DTilemap.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

namespace WFCForge
{
	TiledModel2DTilemap::TiledModel2DTilemap()
	{
	}

	TiledModel2DTilemap::~TiledModel2DTilemap()
	{
	}

	void TiledModel2DTilemap::Clean()
	{
		this->isPrepared = false;
		this->hasTotallyCollapsed = false;
	}

	void TiledModel2DTilemap::BakeToTexture(Texture2D* tex)
	{
		for (int i = 0; i < countY; i++)
		{
			for (int j = 0; j < countX; j++)
			{
				if (tiles[i * countX + j].size() == 1)
					tex->UploadData(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY, tileset.tiles[tiles[i * countX + j].back()].GetDataPTR());
				else
					tex->UploadData(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY, blankTex.data());
			}
		}
	}

	void TiledModel2DTilemap::Prepare(int countX_, int countY_, int tileSizeX_, int tileSizeY_, TiledModel2DTileset* tileset_, bool wrapOutput_)
	{
		this->wrapOutput = wrapOutput_;
		this->countX = countX_;
		this->countY = countY_;
		this->tileSizeX = tileSizeX_;
		this->tileSizeY = tileSizeY_;
		this->tileset = *tileset_;

		this->tiles = std::vector<std::vector<int>>(countX * countY);
		for (auto i = 0; i < countX * countY; i++) this->tiles[i] = tileset.hashes;
		if (tileSizeX * tileSizeY * 4 > blankTex.size())
			while (tileSizeX * tileSizeY * 4 < blankTex.size())
				blankTex.push_back(0);

		items.clear();
		for (int i = 0; i < countX * countY; i++) items.push_back(i);

		this->isPrepared = true;
		this->hasTotallyCollapsed = false;
	}

	void TiledModel2DTilemap::Collapse(int x, int y, int id)
	{
		int index = y * countX + x;

		if (tiles[index].size() > 1)
		{
			auto temp = tiles[index][id];
			tiles[index].clear();
			tiles[index].push_back(temp);
			this->PropagateFor(x, y);
		}
	}

	void TiledModel2DTilemap::CollapseNext()
	{
		// sort according to entropy
		std::sort(items.begin(), items.end(), [this](const int& lhs, const int& rhs)->bool
			{
				// return this->tiles[items[lhs]].tiles.size() < this->tiles[items[rhs]].tiles.size();
				return this->tiles[lhs].size() < this->tiles[rhs].size();
			});

		// TODO do this in a better way
		int co = 0;
		while (co < items.size()) if (tiles[items[co++]].size() > 1) break;
		if (co == items.size()) // all collapsed
		{
			hasTotallyCollapsed = true;
			return;
		}
		co--;
		// this part is just for randomness
		auto minEntropy = tiles[items[co]].size();
		auto minEntropyID = co;
		while (co < items.size()) if (tiles[items[co++]].size() != minEntropy) break;
		co--;
		auto selectedIndex = items[rand() % (co - minEntropyID) + minEntropyID];
		auto selectedY = selectedIndex / countX;
		auto selectedX = selectedIndex % countX;
		auto selectedTileId = rand() % tiles[selectedIndex].size();
		Collapse(selectedX, selectedY, (int)selectedTileId);
	}

	void TiledModel2DTilemap::CollapseAll(int seed)
	{
		srand(seed);
		while (!this->hasTotallyCollapsed) this->CollapseNext();
	}

	void TiledModel2DTilemap::ExportImage(std::string path)
	{
		int w = countX * tileSizeX;
		int h = countY * tileSizeY;
		unsigned char* d = new unsigned char[w * h * 4];
		for (int i = 0; i < countY; i++)
		{
			for (int j = 0; j < countX; j++)
			{
				int tileIndex = i * countY + j;
				int startX = j * tileSizeX;
				int startY = i * tileSizeY;
				for (int a = startY; a < startY + tileSizeY; a++)
				{
					for (int b = startX; b < startX + tileSizeX; b++)
					{
						int dInd = (a * w + b) * 4;
						int tInd = ((a - startY) * tileSizeX + (b - startX)) * 4;
						auto dataPTR = tileset.tiles[tiles[tileIndex].back()].GetDataPTR();
						if (tiles[tileIndex].size() <= 0) dataPTR = blankTex.data();
						d[dInd + 0] = dataPTR[tInd + 0];
						d[dInd + 1] = dataPTR[tInd + 1];
						d[dInd + 2] = dataPTR[tInd + 2];
						d[dInd + 3] = dataPTR[tInd + 3];
					}
				}

			}
		}
		stbi_flip_vertically_on_write(false);
		stbi_write_png(path.data(), w, h, 4, d, w * 4);
		WFC_SAFE_DELETE(d);
	}

	void TiledModel2DTilemap::PropagateFor(int x, int y)
	{
		// TODO: maybe try to reduce code repetations here
		int index = y * countX + x;
		int nx = 0, ny = 0;

		if (tiles[index].size() == 0) return; // nothing to propagate	

		// top
		nx = x; ny = y - 1;
		if (wrapOutput) if (ny < 0)  ny = countY - 1;
		if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
			ReduceEntropy(nx, ny, tiles[index].back(), WFC_TM2D_TILE_TOP);

		// right
		nx = x + 1; ny = y;
		if (wrapOutput) if (nx >= countX)  nx = 0;
		if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
			ReduceEntropy(nx, ny, tiles[index].back(), WFC_TM2D_TILE_RIGHT);

		// bottom
		nx = x; ny = y + 1;
		if (wrapOutput) if (ny >= countY)  ny = 0;
		if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
			ReduceEntropy(nx, ny, tiles[index].back(), WFC_TM2D_TILE_BOTTOM);


		// left
		nx = x - 1; ny = y;
		if (wrapOutput) if (nx < 0)  nx = countX - 1;
		if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
			ReduceEntropy(nx, ny, tiles[index].back(), WFC_TM2D_TILE_LEFT);

	}

	void TiledModel2DTilemap::ReduceEntropy(int x, int y, int currentHash, int targetSide)
	{
		auto index = y * countX + x;
		if (tiles[index].size() <= 1) return; // entropy is already minimum
		std::vector<int> ts;
		for (auto i = 0; i < tiles[index].size(); i++)
			if (tileset.tiles[currentHash].IsTileAllowed(tiles[index][i], targetSide))
				ts.push_back(tiles[index][i]);
		tiles[index] = ts;
		if (tiles[index].size() <= 1) // if collapsed
			this->PropagateFor(x, y);
	}
}