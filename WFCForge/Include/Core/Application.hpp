#pragma once

#include "Core/Core.hpp"
#include "Core/Window.hpp"

namespace WFCForge
{

	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		void Start(int argc, char** argv);
		void Run();
		void End();
		void Close();
		void OnCloseProxy(); // maybe temp

	public:

		inline bool IsRunning() { return this->isRunning; }

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnUIShow() = 0;
		virtual void OnEnd() = 0;
		virtual bool OnClose() = 0;

	protected:
		Window mainWindow;
		std::vector<std::string> arguments;
		bool isRunning = false;
	};

}