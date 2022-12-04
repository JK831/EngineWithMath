#include "pch.h"
#include "Resources.h"

void Resources::RegisterAssets()
{
	wstring assetPath = L"..\\Resources";

	fs::directory_iterator itr(assetPath);
	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;
		if (fs::is_directory(entry.path()))
			ParseAssetFiles(entry.path());
		itr++;
	}
}

void Resources::ParseAssetFiles(fs::path InPath)
{
	wstring ext = InPath.extension();

	FILE* texture = NULL;

	if (0 != _wfopen_s(&texture, InPath.c_str(), L"rb"))
	{
		return;
	}

	uint32 key;
	uint16 objectID;

	if (ext == L".mat")
	{
		key = static_cast<uint8>(OBJECT_TYPE::MATERIAL);
	}
	else if (ext == L".fbx" || ext == L".mesh")
	{
		key = static_cast<uint8>(OBJECT_TYPE::MESH);
	}
	else if (ext == L".hlsl" || ext == L".hlsli")
	{
		key = static_cast<uint8>(OBJECT_TYPE::SHADER);
	}
	else if (ext == L".h")
	{
		key = static_cast<uint8>(OBJECT_TYPE::COMPONENT);
	}
	else if (ext != L".xml")
	{
		key = static_cast<uint8>(OBJECT_TYPE::MATERIAL);
	}

	objectID = _path[key].size() + 1; // 해당 오브젝트 맵의 사이즈를 기반으로 ID 부여
	wstring stringKey = to_wstring((key << _fileIDSize) + objectID);
	_path[key][stringKey] = InPath;
}

shared_ptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr findMesh = Get<Mesh>(L"Rectangle");
	if (findMesh)
		return findMesh;

	float halfWidth = 0.5f;
	float halfHeight = 0.5f;

	vector<Vertex> vertices(4);

	vertices[0] = Vertex(Vector2(-halfWidth, halfHeight), Vector2(0.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[1] = Vertex(Vector2(halfWidth, halfHeight), Vector2(1.f, 0.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[2] = Vertex(Vector2(halfWidth, -halfHeight), Vector2(1.f, 1.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));
	vertices[3] = Vertex(Vector2(-halfWidth, -halfHeight), Vector2(0.f, 1.f), Vector2(0.f, 0.f), Vector2(0.f, 0.f));


	vector<uint32> idx(6);

	idx[0] = 0;	idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertices, idx);
	Add(L"Rectangle", mesh);

	return mesh;
}