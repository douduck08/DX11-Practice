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

	auto pMainCamera = scene.CreateCamera(win.GetGraphics(), "MainCamera");
	scene.SetMainCamera(pMainCamera);
	pMainCamera->SetPosition(-100, 10, 0);
	pMainCamera->SetRotation(-10, 90, 0);

	auto pCamera = scene.CreateCamera(win.GetGraphics(), "Camera 2");
	pCamera->SetPosition(0, 200, 0);
	pCamera->SetRotation(90, 00, 0);
	
	auto cube = Geometry::GenerateCube();
	auto pCubeNode = scene.GetRootNode()->CreateChildNode("Cubes");
	pCubeNode->SetPosition(0, 50, 0);
	pCubeNode->SetScale(3, 3, 3);
	for (int i = 0; i < 20; i++) {
		auto pModel = scene.CreateModel(win.GetGraphics(), pCubeNode, "Cube " + std::to_string(i));
		pModel->SetGeometry(win.GetGraphics(), "Cube", cube);
		pModel->SetPosition(unif(generator), unif(generator), unif(generator));
	}

	//scene.LoadModelFromFile(win.GetGraphics(), "suzanne", "Models/suzanne.obj");
	auto suzanneNode = AssimpKit::LoadModelFromFile(win.GetGraphics(), scene, "suzanne", "Models/suzanne.obj");
	suzanneNode->SetPosition(0, 20, 0);
	suzanneNode->SetRotation(0, 90, 0);
	suzanneNode->SetScale(10, 10, 10);

	//scene.LoadModelFromFile(win.GetGraphics(), "sponza", "Models/Sponza/sponza.obj");
	auto sponzaNode = AssimpKit::LoadModelFromFile(win.GetGraphics(), scene, "sponza", "Models/Sponza/sponza.obj");
	sponzaNode->SetScale(0.1, 0.1, 0.1);

	auto pLightsNode = scene.GetRootNode()->CreateChildNode("Lights");
	auto pLight = scene.CreateLight(win.GetGraphics(), pLightsNode, "Directional");
	pLight->SetLightType(win.GetGraphics(), LightType::Directional, true);
	pLight->SetIntensity(0.8f);
	pLight->SetRotation(-75, -50, 0);

	pLight = scene.CreateLight(win.GetGraphics(), pLightsNode, "Point R");
	pLight->SetColor(1.0f, 0.0f, 0.0f);
	pLight->SetIntensity(2.0f);
	pLight->SetPosition(0, 10, 20);

	pLight = scene.CreateLight(win.GetGraphics(), pLightsNode, "Point G");
	pLight->SetColor(0.0f, 1.0f, 0.0f);
	pLight->SetIntensity(2.0f);
	pLight->SetPosition(-30, 10, 20);

	pLight = scene.CreateLight(win.GetGraphics(), pLightsNode, "Point B");
	pLight->SetColor(0.0f, 0.0f, 1.0f);
	pLight->SetIntensity(2.0f);
	pLight->SetPosition(-60, 10, 20);

	scene.RecalculateNodeId();
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