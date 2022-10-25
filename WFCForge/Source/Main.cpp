#include "WFCForge.hpp"
#include "MainApplication.hpp"

int main(int argc, char** argv, char** envp)
{
	WFCForge::SharedPtr<WFCForge::Application> app = new WFCForge::MainApplication();

	app->Start(argc, argv);
	app->Run();
	app->End();
}