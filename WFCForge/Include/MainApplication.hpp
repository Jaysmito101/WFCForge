#pragma once

#include "Core/Core.hpp"
#include "WFCForge.hpp"
#include "Data/AppState.hpp"

namespace WFCForge
{

	class MainApplication : public Application
	{
	public:
		MainApplication() = default;
		virtual ~MainApplication() = default;

	protected:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnUIShow() override;
		virtual void OnEnd() override;
		virtual bool OnClose() override;

	private:
		void ShowSettingsFor2D();
		void ShowSettingsFor3D();

	private:
		AppState appState;
	};

}
