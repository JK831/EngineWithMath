// Engine.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "Engine.h"
#include "Input.h"
#include "Timer.h"
#include "Resources.h"
#include "SceneManager.h"

void Engine::Init(const WindowInfo& info)
{
	_renderQueue = make_shared<RenderQueue>();

	ResizeWindow(info);

	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Resources)->Init(InitAssetPath);
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	_renderQueue->Render();

	//ShowFps();
}

void Engine::Render()
{
}

void Engine::RenderBegin()
{
}

void Engine::RenderEnd()
{
}

void Engine::ResizeWindow(const WindowInfo& info)
{
	_renderQueue->Init(info);

	RECT rect = { 0, 0, info.width, info.height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	bool tof = ::SetWindowPos(_renderQueue->GetWindow().hwnd, 0, 400, 400, info.width, info.height, 0);
}

WindowInfo& Engine::GetWindow()
{
	return _renderQueue->GetWindow();
}

void Engine::ShowFps()
{
}
