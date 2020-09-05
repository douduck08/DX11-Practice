#include "Scene.h"
#include "Geometry.h"
#include "imgui/imgui.h"

Scene::Scene(Graphics& graphics)
	: backcolor{ 0.2f, 0.8f, 0.8f }
{
	camera = std::make_unique<Camera>(graphics, 0.3f * 3.1415926f, 1280.f / 720.f, 0.1f, 100);
	light = std::make_unique<Light>(graphics, 1.0f, 1.0f, 1.0f);
	light->SetPosition(0, 10, 0);
}

float* Scene::GetBackcolor()
{
	return backcolor;
}

void Scene::Draw(Graphics& graphics)
{
	// ImGui
	ImGui::ColorEdit3("Backcolor", backcolor);
	camera->ShowImguiWindow();
	light->ShowImguiWindow();

	// draw
	camera->Bind(graphics);
	light->Bind(graphics);

	for (auto& m : models)
	{
		m->Draw(graphics);
	}
}

void Scene::AddModel(std::unique_ptr<Model> model)
{
	models.push_back(std::move(model));
}
