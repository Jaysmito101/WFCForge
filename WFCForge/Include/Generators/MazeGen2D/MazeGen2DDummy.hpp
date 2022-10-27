#pragma once
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"

namespace WFCForge
{

	class MazeGen2DDummy : public MazeGen2DAlgorithm
	{
	public:
		MazeGen2DDummy() = default;
		virtual ~MazeGen2DDummy() = default;

		void Step() override;
		void OnSetup() override;
		void OnDestroy() override;

	private:
		int x = 0;
		int y = 0;
	};

}