#pragma once
#include "Core/Core.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{

	enum Mode
	{
		Mode_TiledModel2D = 0,
		Mode_OverlappedModel2D,
		Mode_MazeGen2D,
		Mode_3D,
		Mode_VoxoMan3D,
		Mode_Unknown
	};

	class AppState
	{
	public:
		AppState() = default;
		virtual ~AppState() = default;

	public:
		Dockspace dockspace;

		Viewport mainViewport;

		struct
		{
			uint32_t _0 = 0;
			uint32_t _1 = 0;
			uint32_t _2 = 0;
		} mousePickID;

		struct
		{
			bool right = false;
			bool middle = false;
			bool left = false;
		} mouseButton;

		struct
		{
			float x = -1.0f;
			float y = -1.0f;
		} mousePosition;

		float scrollDelta = 0.0f;
		float aspectRatio = 1.0f;
		float deltaTime = 0.0f;
		bool is3DMode = false;

		TiledModel2DManager tiledModel2D;
		OverlappedModel2DManager overlappedModel2D;
		MazeGen2DManager mazeGen2D;
		VoxoMan3DManager voxoMan3D;

		Mode mode = Mode_TiledModel2D;

		glm::vec3 temp = glm::vec3(0.0f);

	private:
	};

}
