#pragma once

#include "Material.h"

// Material header include Shader.h

class WindowsRSI;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	void Init(const WindowInfo& info);
	void Render();
	void ResizeWindow(const WindowInfo& info);
	std::vector<Vertex> VertexShading(const::std::vector<Vertex>& InVertexBuffer, const Matrix4x4& InMatrix);
	LinearColor PixelShading(LinearColor UVColor);
	void DrawMesh();
	void DrawTriangle2D(const ::std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture);
	void DrawTriangle3D(const ::std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture);
	uint16 PushMaterial(uint16 InIndexNum, Material& InMaterial);
	void DrawIndexedInstance(const ::std::vector<Vertex>& InVertexBuffer, const std::vector<uint32>& InIndexBuffer, const Matrix4x4& InMatrix, uint16 InBufferIndex);


	WindowInfo& GetWindow() { return _RSIPtr->GetWindow(); }
	HWND& GetHwnd() { return GetWindow().hwnd; }
private:
	unique_ptr <WindowsRSI> _RSIPtr;
	LinearColor _backGroundColor = LinearColor::Black;
	std::vector<::std::shared_ptr<Material>> _materialQueue;

};

