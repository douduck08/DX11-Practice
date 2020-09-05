#pragma once
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Model.h"

class Scene
{
public:
	Scene(Graphics& graphics);
	float* GetBackcolor();
	void Draw(Graphics& graphics);
	void AddModel(std::unique_ptr<Model> model);

private:
	float backcolor[3];

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	std::vector<std::unique_ptr<Model>> models;
};