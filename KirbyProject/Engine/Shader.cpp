#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{

}

Shader::~Shader()
{

}

void Shader::Init(const wstring& path)
{
	CreateVertexShader(path, "VS_Main");
	CreatePixelShader(path, "PS_Main");
}

void Shader::Update()
{

}

std::vector<Vertex> Shader::VertexShading(const std::vector<Vertex>& InVertexBuffer, const Matrix3x3& InMatrix)
{
	return _vertexFunction(InVertexBuffer, InMatrix);
}

LinearColor Shader::PixelShading(LinearColor UVColor)
{
	return _pixelFunction(UVColor);
}

void Shader::CreateShader(const wstring& path, const string& name, const string& version)
{

}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version = "VS_0_0")
{
	_vertexFunction = VertexShader2D;
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version = "PS_0_0")
{
	_pixelFunction = FragmentShader2D;
}