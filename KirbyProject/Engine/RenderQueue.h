#pragma once

#include "Material.h"

// Material header include Shader.h

class WindowsRSI;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	std::vector<Vertex> VertexShading(const::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor PixelShading(LinearColor UVColor, LinearColor InDefaultColor);
	void DrawMesh();
	void DrawTriangle2D(const ::std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture);
	uint16 PushMaterial(::std::shared_ptr<Material> InMaterial);
	void DrawIndexedInstance(const ::std::vector<Vertex>& InVertexBuffer, const std::vector<uint32>& InIndexBuffer, const Matrix3x3& InMatrix, uint16 InBufferIndex);
private:
	unique_ptr <WindowsRSI> _RSIPtr;
	std::vector<::std::shared_ptr<Material>> _materialQueue;

};

