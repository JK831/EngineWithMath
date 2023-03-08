#pragma once
#include "Object.h"

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path);
	void Update();
	std::vector<Vertex> VertexShading(const ::std::vector<Vertex>& InVertexBuffer, const Matrix4x4& InMatrix);
	LinearColor PixelShading(LinearColor UVColor);

private:
	void CreateShader(const wstring& path, const string& name, const string& version);
	void CreateVertexShader(const wstring& path, const string& name, const string& version = "VS_0_0");
	void CreatePixelShader(const wstring& path, const string& name, const string& version = "PS_0_0");

private:
	LinearColor _ownColor = LinearColor::White;
	std::vector<Vertex>(*_vertexFunction) (const std::vector<Vertex>& InVertexBuffer, const Matrix4x4& InMatrix);
	LinearColor(*_pixelFunction) (LinearColor& UVColor, const LinearColor& InColorParam);
};


// 정점 변환 코드
FORCEINLINE std::vector<Vertex> VertexShader3D(const std::vector<Vertex>& InVertices, const Matrix4x4& InMatrix)
{
	std::vector<Vertex> newVertices = InVertices;
	// 위치 값에 최종 행렬을 적용해 변환
	for (Vertex& v : newVertices)
	{
		v.pos = v.pos * InMatrix;
	}

	return newVertices;
}

// 픽셀 변환 코드
FORCEINLINE LinearColor FragmentShader3D(LinearColor& InColor, const LinearColor& InColorParam)
{
	return InColor * InColorParam;
}