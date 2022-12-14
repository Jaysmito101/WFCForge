#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"
#include "WFCForge.hpp"


namespace WFCForge
{
	void MazeGen2DAlgorithm::OnSetup() {}
	
	void MazeGen2DAlgorithm::OnDestroy() {}

	bool MazeGen2DAlgorithm::At(int x, int y, bool wrap)
	{
		if (wrap)
		{
			if (x < 0) x = width - 1;
			if (x >= width) x = 0;
			if (y < 0) y = height - 1;
			if (y >= height) y = 0;
		}
		if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return false;
		return board[y * width + x];
	}

	void MazeGen2DAlgorithm::Setup(int w, int h, int s) 
	{
		if (this->board != nullptr) this->Destroy();
		this->seed = s;
		this->width = w;
		this->height = h;
		srand(this->seed);
		this->board = new bool[w * h];
		this->finished = false;
		this->OnSetup();
	}

	void MazeGen2DAlgorithm::Destroy()
	{
		WFC_SAFE_DELETE(this->board);
		srand(time(NULL)); // optional
		this->OnDestroy();
	}

	std::vector<std::pair<int, int>> MazeGen2DAlgorithm::GetNeighbours(int x, int y, bool isWall)
	{
		std::vector<std::pair<int, int>> out;
		int nx, ny;

		nx = x + 2; ny = y;
		if (nx < width) if (board[ny * width + nx] == isWall) out.push_back({nx, ny});

		nx = x - 2; ny = y;
		if (nx >= 0) if (board[ny * width + nx] == isWall) out.push_back({ nx, ny });

		nx = x; ny = y + 2;
		if (ny < height) if (board[ny * width + nx] == isWall) out.push_back({ nx, ny });

		nx = x; ny = y - 2;
		if (ny >= 0) if (board[ny * width + nx] == isWall) out.push_back({ nx, ny });

		return out;
	}
}