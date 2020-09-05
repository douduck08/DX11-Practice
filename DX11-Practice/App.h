#pragma once
#include <DirectXMath.h>
#include "ImguiManager.h"
#include "Window.h"
#include "DTimer.h"
#include "Scene.h"

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
	Scene scene;
};