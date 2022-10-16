#pragma once

class Material;

class RenderQueue
{
public:
	void VertexShading();
	void PixelShading();
	void DrawMesh();
	void DrawTriangle();
	uint16 PushMaterial(std::shared_ptr<Material> InMaterial);
	void DrawIndexedInstance(const std::vector<Vertex>& InVertexBuffer, const std::vector<uint32>& InIndexBuffer, const JK::Matrix3x3& InMatrix, uint16 InBufferIndex);
private:
	std::vector<std::shared_ptr<Material>> _materialQueue;

};

