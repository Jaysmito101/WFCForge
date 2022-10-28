#pragma once
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"
#include <stack>

namespace WFCForge
{

	class MazeGen2DBacktracking : public MazeGen2DAlgorithm
	{
	public:
		MazeGen2DBacktracking() = default;
		virtual ~MazeGen2DBacktracking() = default;

		void Step() override;
		void OnSetup() override;
		void OnDestroy() override;

	private:
		std::stack<std::pair<int, int>> positions;
		std::vector<int> boardVisits;
	};

}