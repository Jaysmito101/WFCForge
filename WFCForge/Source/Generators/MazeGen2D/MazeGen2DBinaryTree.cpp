#include "Generators/MazeGen2D/MazeGen2DBinaryTree.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
	void MazeGen2DBinaryTree::Step()
	{
		if (finished) return;

		// set self to path
		board[y * width + x] = 0;
		// one neighbour to path
		int nx = x, ny = y;
		if (rand() % 2 == 0) ny++;
		else nx++;
		if (nx >= width) nx = 0;
		if (ny >= height) ny = 0;
		if (finished) return;
		board[ny * width + nx] = 0;
		x += 2;
		if (x >= width) { x = 0; y += 2; }
		if (y >= height) finished = true;
	}

	void MazeGen2DBinaryTree::OnSetup()
	{
		memset(board, 1, width * height);
		x = y = 0;
	}

	void MazeGen2DBinaryTree::OnDestroy()
	{

	}

}