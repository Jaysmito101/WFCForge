#include "Core/Core.hpp"
#include "Core/Window.hpp"

#include <GLFW/glfw3.h>

namespace WFCForge
{
	static uint32_t s_ActiveWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		WFC_LOG_ERROR("GLFW Error: %d - %s", error, description);
	}

	static void InitializeGLFW()
	{
		if (!glfwInit())
		{
			WFC_LOG_ERROR("Error in initializeing GLFW!");
			WFC_ABRUPT_EXIT(-1);
		}
		glfwSetErrorCallback(GLFWErrorCallback);
		WFC_LOG("GLFW Initialized");
	}

	static void ShutdownGLFW()
	{
		glfwTerminate();
		WFC_LOG("GLFW Shutdown");
	}

	Window::Window()
	{
	}

	Window::~Window()
	{
		if (this->handle)
			this->Destory();
	}
	

	void Window::SwapBuffers()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwSwapBuffers(this->handle);
	}

	void Window::PollEvents()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwPollEvents();
	}

	void Window::MakeContextCurrent()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwMakeContextCurrent(this->handle);
	}

	void Window::SetTitle(const std::string& title)
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwSetWindowTitle(this->handle, title.data());
	}

	void Window::SetSize(uint32_t width, uint32_t height)
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwSetWindowSize(this->handle, width, height);
	}

	void Window::SetPosition(uint32_t x, uint32_t y)
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwSetWindowPos(this->handle, x, y);
	}

	void Window::SetVisible(bool value)
	{
		WFC_ASSERT(this->handle, "Window not setup");
		this->isVisible = value;
		if(this->isVisible)
			glfwShowWindow(this->handle);
		else
			glfwHideWindow(this->handle);
	}

	void Window::Setup()
	{
		WFC_ASSERT(!this->handle, "Window is already setup");

		if (s_ActiveWindowCount == 0)
			InitializeGLFW();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		// TerraForge3D uses OpenGL 4.3 (will be updated to 4.6 later)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		this->handle = glfwCreateWindow(800, 600, "WINDOW", nullptr, nullptr);

		if (!this->handle)
		{
			WFC_LOG_ERROR("Failed to create window");
			WFC_ABRUPT_EXIT(-1);
		}

		glfwSetWindowUserPointer(this->handle, this);

		glfwSetWindowCloseCallback(this->handle, [](GLFWwindow* handle) -> void {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
			window->OnClose();
			});

		glfwMakeContextCurrent(this->handle);

		s_ActiveWindowCount++;

	}

	void Window::Destory()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		glfwDestroyWindow(this->handle);
		this->handle = nullptr;

		s_ActiveWindowCount--;
		if (s_ActiveWindowCount == 0)
			ShutdownGLFW();
	}

	void Window::OnClose()
	{
		if (this->onClose)
			this->onClose();
	}

	std::pair<uint32_t, uint32_t> Window::GetSize()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		int w = 0, h = 0;
		glfwGetWindowSize(this->handle, &w, &h);
		return {w, h};
	}

	std::pair<uint32_t, uint32_t> Window::GetPosition()
	{
		WFC_ASSERT(this->handle, "Window not setup");
		int x = 0, y = 0;
		glfwGetWindowPos(this->handle, &x, &y);
		return { x, y };
	}

}