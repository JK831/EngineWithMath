// Engine.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "Engine.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

void Engine::Init(const WindowInfo& info)
{
	
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	Render();

	ShowFps();
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

void Engine::ResizeWindow(int32 width, int32 height)
{
}

void Engine::ShowFps()
{
}
