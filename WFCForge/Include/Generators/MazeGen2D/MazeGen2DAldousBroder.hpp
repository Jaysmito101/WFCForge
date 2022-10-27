#pragma once
#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"

namespace WFCForge
{

	class MazeGen2DAldousBroder : public MazeGen2DAlgorithm
	{
	public:
		MazeGen2DAldousBroder() = default;
		virtual ~MazeGen2DAldousBroder() = default;

		void Step() override;
		void OnSetup() override;
		void OnDestroy() override;

	private:
		int x = 0;
		int y = 0;
		int numVisited = 0;
	};

}