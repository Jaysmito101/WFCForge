#include "Generators/MazeGen2D/MazeGen2DAldousBroder.hpp"
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
	void MazeGen2DAldousBroder::Step()
	{
		if (finished) return;

		auto neigh = GetNeighbours(x, y, true);
		if (neigh.size() == 0) // means all neighbours are already visited
		{
			neigh = GetNeighbours(x, y, false);
			auto randNeigh = neigh[rand() % neigh.size()];
			x = randNeigh.first;
			y = randNeigh.second;
			return;
		}
		auto randNeigh = neigh[rand() % neigh.size()];
		int nx = randNeigh.first;
		int ny = randNeigh.second;
		board[(ny + y)/2 * width + (nx + x)/2] = 0;
		board[ny * width + nx] = 0;
		x = nx; y = ny;
		numVisited++;
		if (numVisited >= (width * height) / 4) finished = true;
	}

	void MazeGen2DAldousBroder::OnSetup()
	{
		memset(board, 1, width * height);

		x = rand() % width;
		y = rand() % height;

		board[y * width + x] = 0;

		numVisited = 1;
	}

	void MazeGen2DAldousBroder::OnDestroy()
	{

	}

}