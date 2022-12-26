#include "pch.h"
#include "Resources.h"
#include "tinyxml2.h"

void Resources::Init(wstring assetPath)
{
	// TODO: ���� �о����

	fs::recursive_directory_iterator itr(assetPath);
	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;
		if (fs::is_regular_file(entry.path()))
			ParseAssetFiles(entry.path());
		// TODO: AssetFile�� meta ������ ���� �� ������ֱ�
		itr++;
	}
}

void Resources::CheckAssets(wstring assetPath)
{
	{
		std::map<std::wstring, std::wstring>::iterator itr = _IDPathMap.begin();

		while (itr != _IDPathMap.end())
		{
			if (fs::exists(fs::path(itr->second)) == false)
			{
				_IDPathMap.erase(itr);
				_pathIDMap.erase(itr->second);
			}
			itr++;
		}
	}
	{
		fs::recursive_directory_iterator itr(assetPath);
		while (itr != fs::end(itr))
		{
			const fs::directory_entry& entry = *itr;
			if (_pathIDMap.find(entry.path()) == _pathIDMap.end())
			{
				ParseAssetFiles(entry.path());
			}
			
			itr++;
		}
	}
}

void Resources::MakeMetaFile(wstring assetPath)
{

}

OBJECT_TYPE Resources::GetObjectTypeByExt(wstring& filePath)
{
	wstring ext = fs::path(filePath).extension();
	
	if (ext == L".mat")
	{
		return OBJECT_TYPE::MATERIAL;
	}
	else if (ext == L".fbx" || ext == L".mesh")
	{
		return OBJECT_TYPE::MESH;
	}
	else if (ext == L".hlsl" || ext == L".hlsli")
	{
		return OBJECT_TYPE::SHADER;
	}
	else if (ext == L".h")
	{
		return OBJECT_TYPE::COMPONENT;
	}
	else if (ext != L".xml")
	{
		return OBJECT_TYPE::TEXTURE;
	}
}

void Resources::ParseAssetFiles(fs::path InPath)
{
	wstring ext = InPath.extension();

	if (ext != L".meta")
		return;


	/*tinyxml2::XMLElement* root = sceneName.RootElement();
	tinyxml2::XMLElement* object = root->FirstChildElement();*/

	OBJECT_TYPE objectType;
	uint16 objectID;

	objectType = GetObjectTypeByExt(ext);

	objectID = _objectCount[static_cast<uint8>(objectType)]++; // �ش� ������Ʈ ���� ����� ������� ID �ο�
	wstring stringKey = to_wstring((static_cast<uint32>(objectType) << _fileIDSize) + objectID);
	_IDPathMap[stringKey] = InPath;
	_pathIDMap[InPath] = stringKey;

	switch (objectType)
	{
		bool tof;
	case OBJECT_TYPE::MATERIAL:
	{
		tof = Add(stringKey, make_shared<Material>());
	}
	case OBJECT_TYPE::MESH:
	{
		MakeMetaFile(InPath);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		// meta���Ϸκ��� �����͸� �о�� �ʱ� ���� �� Add
		tof = Add(stringKey, mesh);
	}
	case OBJECT_TYPE::SHADER:
	{
		MakeMetaFile(InPath);
		shared_ptr<Shader> shader = make_shared<Shader>();
		tof = Add(stringKey, shader);
	}
	case OBJECT_TYPE::COMPONENT:
	{
		MakeMetaFile(InPath);
		shared_ptr<MonoBehaviour> monoBehaviour = make_shared<MonoBehaviour>();
		tof = Add(stringKey, monoBehaviour);
	}
	case OBJECT_TYPE::TEXTURE:
	{
		MakeMetaFile(InPath);
		shared_ptr<Texture> texture = make_shared<Texture>();
		tof = Add(stringKey, texture);
	}
	}
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