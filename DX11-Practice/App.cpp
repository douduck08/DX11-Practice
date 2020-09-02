#include "App.h"
#include "imgui/imgui.h"
#include "Geometry.h"


App::App()
	: win(200, 200, 800, 600, "My App")
{
	auto cube = Geometry::GenerateCube();
	model.SetMesh(win.GetGraphics(), cube.vertices, 8, cube.indices, 36u);
}

void App::DoFrame(float t, float dt)
{
	win.GetGraphics().BeginFrame(backcolor.x, backcolor.y, backcolor.z);

	// ImGui
	if (ImGui::Begin("Test"))
	{
		ImGui::ColorEdit3("Backcolor", &backcolor.x);
	}
	ImGui::End();

	// draw
	//win.GetGraphics().DrawTest(t);
	model.Draw(win.GetGraphics(), t);

	// present
	win.GetGraphics().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	static auto show_imgui_demo = true;
	if (show_imgui_demo) {
		ImGui::ShowDemoWindow(&show_imgui_demo);
	}
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
		DoFrame(t, dt);
	}
	return 0;;
}