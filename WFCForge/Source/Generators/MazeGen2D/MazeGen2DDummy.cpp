#include "Generators/MazeGen2D/MazeGen2DDummy.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
	void MazeGen2DDummy::Step()
	{
		if (finished) return;

		board[y * width + x++] = true;
		if (x == width) { x = 0; y++; }
		if (y == height) finished = true;
	}

	void MazeGen2DDummy::OnSetup()
	{
		memset(board, 0, width * height);
		x = y = 0;
	}

	void MazeGen2DDummy::OnDestroy()
	{

	}

}