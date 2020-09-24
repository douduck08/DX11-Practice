#include "App.h"
#include "AssimpKit.h"
#include "imgui/imgui.h"
#include "Geometry.h"
#include "Light.h"
#include "DebugGuiWindow.h"
#include <random>
#include <memory>

App::App()
	: win(1280, 720, "My D3D Engine")
	, scene(win.GetGraphics())
{
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> unif(-5.0, 5.0);

	auto cube = Geometry::GenerateCube();
	auto cubeNode = scene.CreateChildSceneNode("Cubes");
	cubeNode->SetPosition(0, 50, 0);
	cubeNode->SetScale(3, 3, 3);
	for (int i = 0; i < 20; i++) {
		auto childNode = cubeNode->CreateChild("Cube " + std::to_string(i));
		childNode->SetPosition(unif(generator), unif(generator), unif(generator));

		auto model = std::make_unique<Model>(win.GetGraphics(), "Cube", cube);
		scene.AddModel(childNode, std::move(model));
	}

	//scene.LoadModelFromFile(win.GetGraphics(), "suzanne", "Models/suzanne.obj");
	auto suzanneNode = AssimpKit::LoadModelFromFile(win.GetGraphics(), scene, "suzanne", "Models/suzanne.obj");
	suzanneNode->SetPosition(0, 20, 0);
	suzanneNode->SetRotation(0, 90, 0);
	suzanneNode->SetScale(10, 10, 10);

	//scene.LoadModelFromFile(win.GetGraphics(), "sponza", "Models/Sponza/sponza.obj");
	auto sponzaNode = AssimpKit::LoadModelFromFile(win.GetGraphics(), scene, "sponza", "Models/Sponza/sponza.obj");
	sponzaNode->SetScale(0.1, 0.1, 0.1);

	auto pNode = scene.CreateChildSceneNode("Lights");
	auto pLight = std::make_unique<Light>(win.GetGraphics(), LightType::Directional, 1.0f, 1.0f, 1.0f, 0.8f, true);
	auto pChild = pNode->CreateChild("Directional");
	pChild->SetRotation(-75, -50, 0);
	scene.AddLight(pChild, std::move(pLight));

	pLight = std::make_unique<Light>(win.GetGraphics(), LightType::Point, 1.0f, 0.0f, 0.0f, 2.0f);
	pChild = pNode->CreateChild("Point R");
	pChild->SetPosition(0, 10, 20);
	scene.AddLight(pChild, std::move(pLight));

	pLight = std::make_unique<Light>(win.GetGraphics(), LightType::Point, 0.0f, 1.0f, 0.0f, 2.0f);
	pChild = pNode->CreateChild("Point G");
	pChild->SetPosition(-30, 10, 20);
	scene.AddLight(pChild, std::move(pLight));

	pLight = std::make_unique<Light>(win.GetGraphics(), LightType::Point, 0.0f, 0.0f, 1.0f, 2.0f);
	pChild = pNode->CreateChild("Point B");
	pChild->SetPosition(-60, 10, 20);
	scene.AddLight(pChild, std::move(pLight));

	scene.RecalculateId();
}

void App::DoFrame(float t, float dt)
{
	win.GetGraphics().BeginFrame();

	// draw
	scene.Draw(win.GetGraphics());
	DebugGuiWindow::Show(scene);

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