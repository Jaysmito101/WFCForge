#pragma once
#include "Core/Core.hpp"

struct GLFWwindow;

namespace WFCForge
{

	class Window
	{
	public:
		Window();
		virtual ~Window();

		void Setup();
		void SwapBuffers();
		void PollEvents();
		void MakeContextCurrent();
		void SetTitle(const std::string& title);
		void SetSize(uint32_t width, uint32_t height);
		void SetPosition(uint32_t x, uint32_t y);
		void SetVisible(bool value);
		void Destory();
		void OnClose();
		

		std::pair<uint32_t, uint32_t> GetSize();
		std::pair<uint32_t, uint32_t> GetPosition();

		inline GLFWwindow* GetHandle() { return this->handle; }
		inline bool IsVisible() { return this->isVisible; }

		inline void SetOnCloseCallback(std::function<void()> func) { this->onClose = func; }

	private:
		GLFWwindow* handle = nullptr;
		bool isVisible = false;
		std::function<void()> onClose = nullptr;
	};

}