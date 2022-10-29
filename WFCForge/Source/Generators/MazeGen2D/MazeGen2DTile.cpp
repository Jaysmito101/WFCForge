#pragma once
#include "Generators/MazeGen2D/MazeGen2DTile.hpp"

namespace WFCForge
{
	MazeGen2DTile::~MazeGen2DTile()
	{
		this->Destroy();
	}

	MazeGen2DTile::MazeGen2DTile(const MazeGen2DTile& other)
	{
		this->Destroy();
		this->width = other.width;
		this->height = other.width;
		this->data = new unsigned char[width * height * 4];
		memcpy(this->data, other.data, width * height * 4 * sizeof(unsigned char));
		this->tex.LoadFromMemory(width, height, data);
	}

	void MazeGen2DTile::Setup(int w, int h)
	{
		this->Destroy();
		this->width = w;
		this->height = h;
		this->data = new unsigned char[w * h * 4];
		memset(this->data, 0, w * h * 4 * sizeof(unsigned char));
		this->tex.LoadFromMemory(w, h, this->data);
	}

	void MazeGen2DTile::SetData(int w, int h, const unsigned char* d)
	{
		if (w == width && h == height) memcpy(data, d, w * h * 4 * sizeof(unsigned char));
		else
		{
			avir::CImageResizer<> resizer(8);
			resizer.resizeImage(d, w, h, 0, this->data, width, height, 4, 0);
		}
		this->tex.UploadData(0, 0, this->width, this->height, this->data);
	}

	void MazeGen2DTile::Destroy()
	{
		WFC_SAFE_DELETE(data);
		if (tex.IsLoaded()) tex.Destroy();
	}

	void MazeGen2DTile::Rotate()
	{
		int newWidth = height;
		int newHeight = width;
		auto oldData = data;
		data = new unsigned char[this->width * this->height * 4];
		for (auto i = 0; i < this->height; i++)
		{
			for (auto j = 0; j < this->width; j++)
			{
				// data(h - 1 - i, j) = oldData(j, i)
				data[(j * newWidth + (height - i - 1)) * 4 + 0] = oldData[(i * width + j) * 4 + 0];
				data[(j * newWidth + (height - i - 1)) * 4 + 1] = oldData[(i * width + j) * 4 + 1];
				data[(j * newWidth + (height - i - 1)) * 4 + 2] = oldData[(i * width + j) * 4 + 2];
				data[(j * newWidth + (height - i - 1)) * 4 + 3] = oldData[(i * width + j) * 4 + 3];
			}
		}
		width = newWidth;
		height = newHeight;
		WFC_SAFE_DELETE(oldData);
		if(tex.IsLoaded()) tex.Destroy();
		tex.LoadFromMemory(width, height, data);
	}

	void MazeGen2DTile::BakeToTexture(int offsetX, int offsetY, int w, int h, unsigned char* d)
	{
		for (int i = 0; i < this->height; i++) for (int j = 0; j < this->width; j++)
		{
			auto bx = j + offsetX;
			auto by = i + offsetY;
			d[(by * w + bx) * 4 + 0] = this->data[(i * this->width + j) * 4 + 0];
			d[(by * w + bx) * 4 + 1] = this->data[(i * this->width + j) * 4 + 1];
			d[(by * w + bx) * 4 + 2] = this->data[(i * this->width + j) * 4 + 2];
			d[(by * w + bx) * 4 + 3] = this->data[(i * this->width + j) * 4 + 3];
		}
	}
	
	void MazeGen2DTile::WriteToOffset(int offsetX, int offsetY, int w, int h, unsigned char* d)
	{
		//if ((w + offsetX) > width || (h + offsetY) > height) return;
		this->tex.UploadData(offsetX, offsetY, w, h, d);
		for (int i = 0; i < h; i++) for (int j = 0; j < w; j++)
		{
			auto bx = j + offsetX;
			auto by = i + offsetY;
			//if (by >= height || bx >= width || bx < 0 || by < 0) continue;
			this->data[(by * this->width + bx) * 4 + 0] = d[(i * w + j) * 4 + 0];
			this->data[(by * this->width + bx) * 4 + 1] = d[(i * w + j) * 4 + 1];
			this->data[(by * this->width + bx) * 4 + 2] = d[(i * w + j) * 4 + 2];
			this->data[(by * this->width + bx) * 4 + 3] = d[(i * w + j) * 4 + 3];
		}
	}
	
	void MazeGen2DTile::Upload()
	{
		if (this->tex.IsLoaded()) this->tex.UploadData(0, 0, width, height, data);
		// else this->tex.LoadFromMemory(width, height, data);
	}
}