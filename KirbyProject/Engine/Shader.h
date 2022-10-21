#pragma once
#include "Object.h"

class Shader : Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path);
	void Update();
	std::vector<Vertex> VertexShading(const::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor PixelShading(LinearColor UVColor);

private:
	void CreateShader(const wstring& path, const string& name, const string& version);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	std::vector<Vertex>(*_vertexFunction) (const::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor(*_pixelFunction) (LinearColor UVColor);
};


// 정점 변환 코드
FORCEINLINE void VertexShader2D(std::vector<Vertex2D>& InVertices, const Matrix3x3& InMatrix)
{
	// 위치 값에 최종 행렬을 적용해 변환
	for (Vertex2D& v : InVertices)
	{
		v.Position = InMatrix * v.Position;
	}
}

// 픽셀 변환 코드
FORCEINLINE LinearColor FragmentShader2D(LinearColor& InColor, const LinearColor& InColorParam)
{
	return InColor * InColorParam;
}
