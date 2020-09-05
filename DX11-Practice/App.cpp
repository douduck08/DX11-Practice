#include "App.h"
#include "imgui/imgui.h"
#include "Geometry.h"
#include <random>

App::App()
	: win(200, 200, 1280, 720, "My D3D Engine")
	, scene(win.GetGraphics())
{
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> unif(-5.0, 5.0);

	auto cube = Geometry::GenerateCube();
	for (int i = 0; i < 20; i++) {
		float x = unif(generator);
		float y = unif(generator);
		float z = unif(generator);

		auto model = std::make_unique<Model>();
		model->SetGeometry(win.GetGraphics(), cube);
		model->SetShader(win.GetGraphics(), L"Shaders/SimpleLitVertexShader.cso", L"Shaders/SimpleLitPixelShader.cso");
		model->SetPositionAndRotation(x, y, z);
		
		scene.AddModel(std::move(model));
	}
}

void App::DoFrame(float t, float dt)
{
	win.GetGraphics().BeginFrame(scene.GetBackcolor());

	scene.Draw(win.GetGraphics());

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