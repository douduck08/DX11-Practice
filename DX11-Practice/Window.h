#pragma once
#include <Windows.h>
#include <memory>
#include <optional>
#include <string>
#include "Graphics.h"

class Window
{
public:
	Window(int x, int y, int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	static const char* GetClassName() noexcept;
	static std::optional<int> ProcessMessages() noexcept;

	HINSTANCE GetInstance() noexcept;
	Graphics& GetGraphics();

	void SetTitle(const std::string& title);

private:
	static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK WindowProcNormal(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:

private:
	static constexpr const char* wndClassName = "Duck Direct3D Engine Window";

	HINSTANCE hInstance;;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGraphics;
};