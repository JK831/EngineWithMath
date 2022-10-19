#pragma once


class RenderQueue
{
public:
	void VertexShading();
	void PixelShading();
	void DrawMesh();
	void DrawTriangle2D(const ::std::vector<Vertex>& InTvs, uint16 InBufferIndex);
	uint16 PushMaterial(::std::shared_ptr<JK::Material> InMaterial);
	void DrawIndexedInstance(const ::std::vector<Vertex>& InVertexBuffer, const std::vector<uint32>& InIndexBuffer, const JK::Matrix3x3& InMatrix, uint16 InBufferIndex);
private:
	std::vector<::std::shared_ptr<JK::Material>> _materialQueue;

};

