#include "Generators/MazeGen2D/MazeGen2DKruskal.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
	void MazeGen2DKruskal::Step()
	{
		if (finished) return;

		// select a random edge
		auto edgeIndex = rand() % edges.size();
		auto edge = edges[edgeIndex];
		edges.erase(edges.begin() + edgeIndex);

		int nx = edge.first, ny = edge.second;
		
		// if edge is horizontal
		if (edge.second % 2 == 0)
		{
			ny += 1;
			if (ny >= height) return;
			// std::pair<int, int> bottom = {nx, ny};
			auto bottomSet = setIds[ny * width + nx];
			ny -= 2;
			if (ny < 0) return;
			// std::pair<int, int> top = { nx, ny };
			auto topSet = setIds[ny * width + nx];
			if (topSet != bottomSet) // join the sets
			{
				ChangeSet(topSet, bottomSet);
				board[edge.second * width + edge.first] = 0;
			}
		}
		// if edge is vertical
		else if (edge.first % 2 == 0)
		{
			nx += 1;
			if (nx >= width) return;
			// std::pair<int, int> bottom = {nx, ny};
			auto rightSet = setIds[ny * width + nx];
			nx -= 2;
			if (nx < 0) return;
			// std::pair<int, int> top = { nx, ny };
			auto leftSet = setIds[ny * width + nx];
			if (leftSet != rightSet) // join the sets
			{
				ChangeSet(leftSet, rightSet);
				board[edge.second * width + edge.first] = 0;
			}
		}

		if (setCount <= 1) finished = true;
	}

	void MazeGen2DKruskal::ChangeSet(int from, int to)
	{
		setCount--;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				if (setIds[i * width + j] == from) setIds[i * width + j] = to;
	}

	void MazeGen2DKruskal::OnSetup()
	{
		memset(board, 1, width * height);
		edges.clear();
		setIds = std::vector<int>(width * height);
		setCount= 0;
		for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
		{
			if (i % 2 == 0 || j % 2 == 0)
			{
				edges.push_back({ i, j });
				setIds[i * width + j] = -1;
			}
			else
			{
				setIds[i * width + j] = setCount++;
				board[i * width + j] = 0;
			}
		}
	}

	void MazeGen2DKruskal::OnDestroy()
	{

	}

}