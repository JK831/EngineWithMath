#include "pch.h"
#include "Mesh.h"
#include "Engine.h"


Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{

}

Mesh::~Mesh()
{

}

void Mesh::Init(const vector<Vertex>& inVertices, const vector<uint32>& inIndices)
{
	_vertexBuffer = inVertices;
	_indexBuffer = inIndices;
}

void Mesh::Render(shared_ptr<Matrix3x3> InMatrix, uint16 InBufferIndex)
{
	// Material이 저장된 버퍼의 인덱스를 가지고 물체 그려줌
	CMD_LIST->DrawIndexedInstance(&_vertexBuffer, &_indexBuffer, &InMatrix, InBufferIndex);
}

void Mesh::CreateVertexBuffer(const vector<Vector2>& buffer)
{
	_vertexBuffer = buffer;
	_vertexCount = static_cast<uint32>(buffer.size());
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	_indexBuffer = buffer;
	_indexCount = static_cast<uint32>(buffer.size());
}
