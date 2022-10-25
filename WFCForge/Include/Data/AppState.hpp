#pragma once
#include "Core/Core.hpp"
#include "WFCForge.hpp"

namespace WFCForge
{
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

		float deltaTime = 0.0f;
		bool is3DMode = false;

		TiledModel2DManager tiledModel2D;
		OverlappedModel2DManager overlappedModel2D;

		glm::vec3 temp = glm::vec3(0.0f);

	private:
	};

}
