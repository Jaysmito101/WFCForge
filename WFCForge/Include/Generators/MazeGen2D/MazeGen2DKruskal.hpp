#pragma once
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"

namespace WFCForge
{

	class MazeGen2DKruskal : public MazeGen2DAlgorithm
	{
	public:
		MazeGen2DKruskal() = default;
		virtual ~MazeGen2DKruskal() = default;

		void Step() override;
		void OnSetup() override;
		void OnDestroy() override;

	private:
		void ChangeSet(int from, int to);

	private:
		int setCount = 0;
		std::vector<std::pair<int, int>> edges;
		std::vector<int> setIds;
	};

}