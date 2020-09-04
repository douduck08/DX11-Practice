#pragma once
#include <DirectXMath.h>
#include "Window.h"
#include "Graphics.h"
#include "DTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

class App
{
public:
	App();
	~App() = default;
	int Run();

private:
	void DoFrame(float t, float dt);
	void ShowImguiDemoWindow();

private:
	ImguiManager imguiManager;
	Window win;
	DTimer timer;
	DTimer deltaTime;

	DirectX::XMFLOAT3 backcolor = DirectX::XMFLOAT3(0.2f, 0.8f, 0.8f);
	Camera camera;
	Light light;
	Model model;
	Model lightProxy;
};