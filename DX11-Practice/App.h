#pragma once
#include "Window.h"
#include "Graphics.h"
#include "DTimer.h"
#include "ImguiManager.h"

class App
{
public:
	App();
	~App();
	int Run();

private:
	void HandleInput(float t, float dt);
	void DrawFrame(float t, float dt);

private:
	ImguiManager imguiManager;
	Window win;
	DTimer timer;
	DTimer deltaTime;
};