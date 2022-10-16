#pragma once

#include "pch.h"

class Engine
{
public:

	void Init(const WindowInfo& info);
	void Update();

public:
	const WindowInfo& GetWindow() { return _window; }
	shared_ptr<RenderQueue> GetRenderQueue() { return _renderQueue; }

public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	void ShowFps();

private:

	WindowInfo _window;
	RenderQueue _renderQueue;
	
};