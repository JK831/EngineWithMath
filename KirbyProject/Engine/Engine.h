#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "RenderQueue.h"

class Engine
{
public:

	void Init(const WindowInfo& info);
	void Update();

public:
	shared_ptr<RenderQueue> GetRenderQueue() { return _renderQueue; }

public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(const WindowInfo& info);
	WindowInfo& GetWindow();
private:
	void ShowFps();

private:
	shared_ptr<RenderQueue> _renderQueue;
	
};