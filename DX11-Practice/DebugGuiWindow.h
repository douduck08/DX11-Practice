#pragma once

class DebugGuiWindow
{
public:
	static void ShowResourceWindow();
	static void ShowSceneWindow(class Scene& scene);

private:
	static void ShowSceneHierarchy(class Scene& scene, class SceneNode*& pSelectedNode);
	static void ShowSceneNode(class SceneNode* node, class SceneNode*& pSelectedNode);
	static void ShowSceneProperties(class Scene& scene, class SceneNode*& pSelectedNode);
};