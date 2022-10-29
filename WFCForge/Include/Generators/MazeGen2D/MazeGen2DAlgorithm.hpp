#pragma once

#include "Core/Core.hpp"

namespace WFCForge
{

	class MazeGen2DAlgorithm
	{
	public:
		MazeGen2DAlgorithm() = default;
		virtual ~MazeGen2DAlgorithm() = default;

		std::vector<std::pair<int, int>> GetNeighbours(int x, int y, bool isWall);
		void Setup(int width, int height, int seed);
		void Destroy();


		virtual void Step() = 0;
		virtual void OnSetup();
		virtual void OnDestroy();

		bool At(int x, int y, bool wrap = true);

		inline bool IsPrepared() { return this->board != nullptr; }
		inline bool IsFinished() { return this->finished; }

	protected:
		int width = 0;
		int height = 0;
		bool* board = nullptr;
		int seed = 42;
		bool finished = false;
	};

}