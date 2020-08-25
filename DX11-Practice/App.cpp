#include "App.h"

App::App()
	:win(200, 200, 640, 480, "My App")
{
}

App::~App()
{
}

void App::HandleInput(float dt)
{
}

void App::DrawFrame(float dt)
{
	const auto t = timer.Peek();
	win.SetTitle(std::to_string(t));
	win.GetGraphics().ClearBuffer(0.2f, 0.8f, 0.8f);
	win.GetGraphics().EndFrame();
}

int App::Run()
{
	while (true)
	{
		if (const auto ecode = win.ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		auto dt = deltaTime.Delta();
		HandleInput(dt);
		DrawFrame(dt);
	}
	return 0;;
}