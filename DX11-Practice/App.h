#pragma once
#include "Window.h"
#include "Graphics.h"
#include "DTimer.h"

class App
{
public:
	App();
	~App();
	int Run();

private:
	void HandleInput(float dt);
	void DrawFrame(float dt);

private:
	Window win;
	DTimer timer;
	DTimer deltaTime;
};