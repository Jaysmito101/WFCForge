#include "Core/Core.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiManager.hpp"

#include <glad/glad.h> // maybe temp
#include <GLFW/glfw3.h>

namespace WFCForge
{

	void Application::Start(int argc, char** argv)
	{
		// Parse the  arguments
		for (int i = 0; i < argc; i++)
			arguments.push_back(std::string(argv[i]));

		WFC_LOGGER_INITIALIZE(Utils::GetExecutableDir() + PATH_SEPERATOR "Log.txt");

		this->mainWindow.Setup();
		this->mainWindow.MakeContextCurrent();
		this->mainWindow.SetOnCloseCallback(std::bind(&Application::OnCloseProxy, this));
		WFC_LOG("Main Window Initialized");

		// Initialize OpenGL
		if (!gladLoadGL())
		{
			WFC_LOG_ERROR("OpenGL (Glad) Initialization failed!");
			WFC_ABRUPT_EXIT(-1);
		}
		WFC_LOG("OpenGL (Glad) Initialized");

		ImGuiManager::InitializeImGui(Utils::GetExecutableDir() + PATH_SEPERATOR "EidtorConfig.ini", this->mainWindow.GetHandle());

		WFC_LOG("Startup Initiated");
		OnStart();
		WFC_LOG("Startup Completed");

		this->mainWindow.SetVisible(true);
	}

	void Application::Run()
	{
		this->isRunning = true;

		static float prevTime = (float)glfwGetTime();
		static float currTime = (float)glfwGetTime();

		WFC_LOG("Starting main application loop");
		while (this->isRunning)
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			currTime = (float)glfwGetTime();
			float deltaTime = currTime - prevTime;
			prevTime = currTime;

			OnUpdate(deltaTime);

			// maybe temp -----------------
			auto [w, h] = mainWindow.GetSize();
			glEnable(GL_BLEND);
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind the default window framebuffer
			glViewport(0, 0, w, h);
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			// maybe temp -----------------
			ImGuiManager::PrepareFrameImGui();
			OnUIShow();
			ImGuiManager::RenderFrameImGui();
			
			this->mainWindow.SwapBuffers();
			this->mainWindow.PollEvents();			
		}
		WFC_LOG("Ending main application loop");
	}

	void Application::End()
	{
		WFC_LOG("Shutdown Initiated");
		OnEnd();

		ImGuiManager::ShutdownImGui();

		mainWindow.Destory();
		WFC_LOG("Main Window Destroyed");

		WFC_LOG("Shutdown Completed");
		WFC_LOGGER_SHUTDOWN();
	}

	void Application::Close()
	{
		this->isRunning = false;
	}

	void Application::OnCloseProxy()
	{
		if (this->OnClose())
			this->Close();
	}

}