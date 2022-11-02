#include "pch.h"
#include "Resources.h"

shared_ptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr findMesh = Get<Mesh>(L"Cube");
	if (findMesh)
		return findMesh;

	float halfWidth = 0.5f;
	float halfHeight = 0.5f;

	vector<Vertex> vertices(4);

	vertices[0] = Vertex(Vector2(-halfWidth, -halfHeight), Vector2(0.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[1] = Vertex(Vector2(halfWidth, -halfHeight), Vector2(0.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[2] = Vertex(Vector2(-halfWidth, halfHeight), Vector2(0.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[3] = Vertex(Vector2(halfWidth, halfHeight), Vector2(0.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));

	vector<uint32> idx(6);

	idx[0] = 0;	idx[1] = 1; idx[2] = 3;
	idx[3] = 0; idx[4] = 3; idx[5] = 2;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertices, idx);
	Add(L"Rectangle", mesh);

	return mesh;
}