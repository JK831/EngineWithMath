#pragma once

class RendererInterface;
class RenderQueue;

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
	std::unique_ptr <RendererInterface> _RSIPtr;
	WindowInfo _window;
	shared_ptr<RenderQueue> _renderQueue;
	
};