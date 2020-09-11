#include "App.h"
#include "imgui/imgui.h"
#include "Geometry.h"
#include <random>

App::App()
	: win(1280, 720, "My D3D Engine")
	, scene(win.GetGraphics())
{
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> unif(-5.0, 5.0);

	//*
	auto cube = Geometry::GenerateCube();
	auto node = std::make_unique<SceneNode>("Cubes");
	for (int i = 0; i < 20; i++) {
		float x = unif(generator);
		float y = unif(generator);
		float z = unif(generator);

		auto childNode = std::make_unique<SceneNode>("Cube " + std::to_string(i));
		childNode->SetPosition(x, y, z);

		auto model = std::make_unique<Model>(
			win.GetGraphics(), cube,
			L"Shaders/SimpleLitVertexShader.cso", L"Shaders/SimpleLitPixelShader.cso"
		);
		model->AttachToNode(childNode.get());
		
		node->AddChild(std::move(childNode));
		scene.AddModel(std::move(model));
	}
	scene.AddSceneNode(std::move(node));
	//*/

	scene.LoadModelFromFile(win.GetGraphics(), "suzanne", "Models/suzanne.obj");
	scene.LoadModelFromFile(win.GetGraphics(), "sponza", "Models/sponza.obj");

	scene.RecalculateId();
}

void App::DoFrame(float t, float dt)
{
	// clear buffers
	win.GetGraphics().BeginFrame(scene.GetBackcolor());

	// draw
	scene.ShowImguiWindow();
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