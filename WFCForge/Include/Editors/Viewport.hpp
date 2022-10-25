#pragma once
#include "Core/Core.hpp"

namespace WFCForge
{
	class AppState;

	class Viewport
	{
	public:
		Viewport() = default;
		virtual ~Viewport() = default;

		void OnShow();

		inline bool IsVisible() { return this->isVisible; }
		inline void SetVisible(bool value) { this->isVisible = value; }
		inline void SetAppState(AppState* as) { this->appState = as; }

		inline std::pair<float, float> GetMousePosition() { return this->mousePosition; };

	private:
		float zoomSpeed = 0.1f;
		float rotationSpeed = 16.0f;
		float movementSpeed = 30.0f;
		std::pair<float, float> mousePosition = {-1.0f, -1.0f};
		AppState* appState;
		bool isVisible;
	};

}