#include "App.h"
#include "imgui/imgui.h"
#include "Geometry.h"

App::App()
	: win(200, 200, 800, 600, "My App")
	, camera(win.GetGraphics(), 30, 800.f / 600.f, 0.1f, 100)
	, light(win.GetGraphics(), 1.0f, 1.0f, 1.0f)
{
	auto cube = Geometry::GenerateCube();
	model.SetGeometry(win.GetGraphics(), cube);
	model.SetShader(win.GetGraphics(), L"Shaders/SimpleLitVertexShader.cso", L"Shaders/SimpleLitPixelShader.cso");

	auto sphere = Geometry::GenerateSphere(0.1f);
	lightProxy.SetGeometry(win.GetGraphics(), sphere);
	lightProxy.SetShader(win.GetGraphics(), L"Shaders/UnlitVertexShader.cso", L"Shaders/UnlitPixelShader.cso");
}

void App::DoFrame(float t, float dt)
{
	const float degree2rad = 3.1415926f / 180.0f;
	static float backcolor[3] = { 0.2f, 0.8f, 0.8f };
	static float lightcolor[3] = { 1.0f, 1.0f, 1.0f };
	static float lightPos[3] = { 0, 5, 0 };
	static float radius = 5, pitch = 0, yaw = 0, roll = 0;

	win.GetGraphics().BeginFrame(backcolor[0], backcolor[1], backcolor[2]);

	// ImGui
	if (ImGui::Begin("Camera"))
	{
		ImGui::ColorEdit3("Backcolor", backcolor);
		ImGui::SliderFloat("Radius", &radius, 0.1f, 20.0f);
		ImGui::SliderFloat("Pitch", &pitch, -89.0f, 89.0f);
		ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderFloat("Roll", &roll, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			radius = 5, pitch = 0, yaw = 0, roll = 0;
		}
	}
	ImGui::End();

	if (ImGui::Begin("Light"))
	{
		ImGui::ColorEdit3("Color", lightcolor);
		ImGui::InputFloat3("Position", lightPos);
		if (ImGui::Button("Reset"))
		{
			lightPos[0] = lightPos[1] = lightPos[2] = 0;
		}
	}
	ImGui::End();

	// draw
	camera.SetCamera(0, 0, 0, radius, pitch * degree2rad, yaw * degree2rad, roll * degree2rad);
	camera.Bind(win.GetGraphics());

	light.SetColor(lightcolor[0], lightcolor[1], lightcolor[2]);
	light.SetPosition(lightPos[0], lightPos[1], lightPos[2]);
	light.Bind(win.GetGraphics());
	lightProxy.UpdateTransform(lightPos[0], lightPos[1], lightPos[2]);
	lightProxy.Draw(win.GetGraphics(), t);

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