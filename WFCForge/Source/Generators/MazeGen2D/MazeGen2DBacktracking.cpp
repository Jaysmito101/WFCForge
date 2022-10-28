#include "Generators/MazeGen2D/MazeGen2DBacktracking.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
	void MazeGen2DBacktracking::Step()
	{
		if (finished) return;

		auto [x, y] = positions.top();

		board[y * width + x] = 0;
		boardVisits[y * width + x] = 0;

		auto neighbours = this->GetNeighbours(x, y, false);
		std::vector<std::pair<int, int>> unvisitedNeighbours;
		for (auto neighbour : neighbours)
			if (boardVisits[neighbour.second * width + neighbour.first] == 1)
				unvisitedNeighbours.push_back(neighbour);
		
		if (unvisitedNeighbours.size() > 0)
		{
			auto randomNeighbour = unvisitedNeighbours[rand() % unvisitedNeighbours.size()];
			boardVisits[randomNeighbour.second * width + randomNeighbour.first] = 0;
			board[(randomNeighbour.second + y) / 2 * width + (randomNeighbour.first + x) / 2] = 0;
			//board[randomNeighbour.second * width + randomNeighbour.first] = 0;
			positions.push(randomNeighbour);
		}
		else positions.pop();

		if (positions.size() == 0) finished = true;
	}

	void MazeGen2DBacktracking::OnSetup()
	{
		memset(board, 1, width * height);
		while (!positions.empty()) positions.pop();
		boardVisits.clear();
		for (int i = 0; i < height; i++) 
			for (int j = 0; j < width; j++)
			{
				boardVisits.push_back(1);
				if (!(i % 2 == 0 || j % 2 == 0))
					board[i * width + j] = 0;
			}
		int cellX = 2 * (rand() % ((width - 1) / 2)) + 1;
		int cellY = 2 * (rand() % ((height - 1) / 2)) + 1;
		positions.push({cellX, cellY});
	}

	void MazeGen2DBacktracking::OnDestroy()
	{

	}

}