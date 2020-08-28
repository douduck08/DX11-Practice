#include "App.h"
#include "imgui/imgui.h"

App::App()
	:win(200, 200, 800, 600, "My App")
{
}

App::~App()
{
}

void App::HandleInput(float t, float dt)
{
}

void App::DrawFrame(float t, float dt)
{
	win.SetTitle(std::to_string(t));

	win.GetGraphics().BeginFrame(0.2f, 0.8f, 0.8f);
	win.GetGraphics().DrawTest(t);
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

		const auto t = timer.Peek();
		const auto dt = deltaTime.Delta();
		HandleInput(t, dt);
		DrawFrame(t, dt);
	}
	return 0;;
}