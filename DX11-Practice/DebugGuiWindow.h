#pragma once

class DebugGuiWindow
{
public:
	static void ShowSceneHierarchy(class Scene& scene);
	static void ShowCameraWindow(class Camera& camera);
	static void ShowLightWindow(class Light& light);
};