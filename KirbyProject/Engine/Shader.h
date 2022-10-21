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


// ���� ��ȯ �ڵ�
FORCEINLINE void VertexShader2D(std::vector<Vertex2D>& InVertices, const Matrix3x3& InMatrix)
{
	// ��ġ ���� ���� ����� ������ ��ȯ
	for (Vertex2D& v : InVertices)
	{
		v.Position = InMatrix * v.Position;
	}
}

// �ȼ� ��ȯ �ڵ�
FORCEINLINE LinearColor FragmentShader2D(LinearColor& InColor, const LinearColor& InColorParam)
{
	return InColor * InColorParam;
}
