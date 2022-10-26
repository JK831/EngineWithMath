#pragma once
#include "Object.h"

class Shader : Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path);
	void Update();
	void VertexShading(::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor PixelShading(LinearColor UVColor);

private:
	void CreateShader(const wstring& path, const string& name, const string& version);
	void CreateVertexShader(const wstring& path, const string& name, const string& version = "VS_0_0");
	void CreatePixelShader(const wstring& path, const string& name, const string& version = "PS_0_0");

private:
	std::vector<Vertex>(*_vertexFunction) (::std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix);
	LinearColor(*_pixelFunction) (LinearColor UVColor);
};


// ���� ��ȯ �ڵ�
FORCEINLINE void VertexShader2D(std::vector<Vertex>& InVertices, const Matrix3x3& InMatrix)
{
	// ��ġ ���� ���� ����� ������ ��ȯ
	for (Vertex& v : InVertices)
	{
		v.pos = InMatrix * v.pos;
	}
}

// �ȼ� ��ȯ �ڵ�
FORCEINLINE LinearColor FragmentShader2D(LinearColor& InColor, const LinearColor& InColorParam)
{
	return InColor * InColorParam;
}
