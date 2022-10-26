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
                if(tiles[i * countX + j].tiles.size() == 1)                
                    tex->UploadData(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY, tiles[i * countX + j].tiles.back().GetDataPTR());
                else
                    tex->UploadData(j * tileSizeX, i * tileSizeY, tileSizeX, tileSizeY, blankTex.data());
            }
        }
    }
    
    void TiledModel2DTilemap::Prepare(int countX_, int countY_, int tileSizeX_, int tileSizeY_, TiledModel2DTileset* tileset, bool wrapOutput_)
    {
        this->wrapOutput = wrapOutput_;
        this->countX = countX_;
        this->countY = countY_;
        this->tileSizeX = tileSizeX_;
        this->tileSizeY = tileSizeY_;

        this->tiles.clear();
        this->tiles = std::vector<TiledModel2DTileset>(countX * countY);
        for (int i = 0; i < tiles.size(); i++) tileset->Clone(&tiles[i]);

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

        if (tiles[index].tiles.size() > 1)
        {

            tiles[index].Collapse(id);
            this->PropagateFor(x, y);
        }
    }
    
    void TiledModel2DTilemap::CollapseNext(int seed)
    {
        // sort according to entropy
        std::sort(items.begin(), items.end(), [this](const int& lhs, const int& rhs)->bool
            {
                // return this->tiles[items[lhs]].tiles.size() < this->tiles[items[rhs]].tiles.size();
                return this->tiles[lhs].tiles.size() < this->tiles[rhs].tiles.size();
            });
        
        // TODO do this in a better way
        int co = 0;
        while (co < items.size()) if (tiles[items[co++]].tiles.size() > 1) break;
        if (co == items.size()) // all collapsed
        {
            hasTotallyCollapsed = true;
            return;
        }
        co--;
        // this part is just for randomness
        auto minEntropy = tiles[items[co]].tiles.size();
        auto minEntropyID = co;
        while (co < items.size()) if (tiles[items[co++]].tiles.size() != minEntropy) break;
        co--;
        auto selectedIndex = items[rand() % (co - minEntropyID) + minEntropyID];
        auto selectedY = selectedIndex / countX;
        auto selectedX = selectedIndex % countX;
        auto selectedTileId = rand() % tiles[selectedIndex].tiles.size();
        Collapse(selectedX, selectedY, (int)selectedTileId);
    }
    
    void TiledModel2DTilemap::CollapseAll(int seed)
    {
        srand(seed);
        while (!this->hasTotallyCollapsed) this->CollapseNext(seed);
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
			if(tiles[tileIndex].tiles.size() > 0)
			{
	                        d[dInd + 0] = tiles[tileIndex].Back()->data[tInd + 0];
        	                d[dInd + 1] = tiles[tileIndex].Back()->data[tInd + 1];
                	        d[dInd + 2] = tiles[tileIndex].Back()->data[tInd + 2];
                        	d[dInd + 3] = tiles[tileIndex].Back()->data[tInd + 3];
			}
			else
			{
	                        d[dInd + 0] = 0;
        	                d[dInd + 1] = 0;
                	        d[dInd + 2] = 0;
                        	d[dInd + 3] = 0;
			}
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
        
        if (tiles[index].tiles.size() == 0) return; // nothing to propagate

        // top
        int topHash = tiles[index].Back()->GetFaceIDHash(0);
        nx = x; ny = y - 1;
        if (wrapOutput) if (ny < 0)  ny = countY - 1;
        if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
            ReduceEntropy(nx, ny, topHash, 2); 

        // right
        int rightHash = tiles[index].Back()->GetFaceIDHash(1);
        nx = x + 1; ny = y;
        if (wrapOutput) if (nx >= countX)  nx = 0;
        if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
            ReduceEntropy(nx, ny, rightHash, 3);


        // bottom
        int bottomHash = tiles[index].Back()->GetFaceIDHash(2);
        nx = x; ny = y + 1;
        if (wrapOutput) if (ny >= countY)  ny = 0;
        if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
            ReduceEntropy(nx, ny, bottomHash, 0);


        // left
        int leftHash = tiles[index].Back()->GetFaceIDHash(3);
        nx = x - 1; ny = y;
        if (wrapOutput) if (nx < 0)  nx = countX - 1;
        if (nx >= 0 && nx < countX && ny >= 0 && ny < countY)
            ReduceEntropy(nx, ny, leftHash, 1);

    }
    
    void TiledModel2DTilemap::ReduceEntropy(int x, int y, int currentHash, int targetSide)
    {
        auto index = y * countX + x;
        if (tiles[index].tiles.size() <= 1) return; // entropy is already minimum
        TiledModel2DTileset ts;
        for (auto i = 0; i < tiles[index].tiles.size(); i++)
            if (tiles[index].tiles[i].GetFaceIDHash(targetSide) == currentHash)
                ts.AddTile(tiles[index].tiles[i], false);
        ts.Clone(&tiles[index]);
        if (tiles[index].tiles.size() <= 1) // if collapsed
            this->PropagateFor(x, y);
    }
}