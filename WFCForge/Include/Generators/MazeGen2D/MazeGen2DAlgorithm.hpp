#pragma once

namespace WFCForge
{

	class MazeGen2DAlgorithm
	{
	public:
		MazeGen2DAlgorithm() = default;
		virtual ~MazeGen2DAlgorithm() = default;

		void Setup(int width, int height, int seed);
		void Destroy();

		virtual void Step() = 0;
		virtual void OnSetup();
		virtual void OnDestroy();

		inline bool At(int x, int y) { return board[y * width + x]; }
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