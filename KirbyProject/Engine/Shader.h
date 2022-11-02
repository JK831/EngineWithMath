#pragma once
#include "Object.h"

class Shader : Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path);
	void Update();
	std::vector<Vertex> VertexShading(const ::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor PixelShading(LinearColor UVColor);

private:
	void CreateShader(const wstring& path, const string& name, const string& version);
	void CreateVertexShader(const wstring& path, const string& name, const string& version = "VS_0_0");
	void CreatePixelShader(const wstring& path, const string& name, const string& version = "PS_0_0");

private:
	std::vector<Vertex>(*_vertexFunction) (const std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor(*_pixelFunction) (LinearColor& UVColor, const LinearColor& InColorParam);
};


// 정점 변환 코드
FORCEINLINE std::vector<Vertex> VertexShader2D(const std::vector<Vertex>& InVertices, const Matrix3x3& InMatrix)
{
	std::vector<Vertex> newVertices;
	newVertices = InVertices;

	// 위치 값에 최종 행렬을 적용해 변환
	for (Vertex v : newVertices)
	{
		v.pos *= InMatrix;
	}
	return newVertices;
}

// 픽셀 변환 코드
FORCEINLINE LinearColor FragmentShader2D(LinearColor& InColor, const LinearColor& InColorParam)
{
	return InColor * InColorParam;
}
