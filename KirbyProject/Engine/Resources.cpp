#include "pch.h"
#include "Resources.h"
#include "tinyxml2.h"
#include "guid.hpp"
#include <combaseapi.h>
#include <fstream>

enum class ImporterType
{
	DEFAULT = 1,
	MONO,
};

struct MetaFormat
{
	GUID guid;
	ImporterType _ImporterType;
};

void Resources::Init(wstring assetPath)
{
	// TODO: 파일 읽어오기

	fs::recursive_directory_iterator itr(assetPath);
	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;
		if (fs::is_regular_file(entry.path()))
			ParseAssetFiles(entry.path());
		// TODO: AssetFile이 meta 파일이 없을 시 만들어주기
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

void Resources::MakeMeta(wstring assetPath)
{
	fs::recursive_directory_iterator itr(assetPath);
	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;
		if (fs::is_regular_file(entry.path()))
		{
			wstring ext = fs::path(entry.path()).extension();
			if (ext == L".meta")
				continue;
			MakeMetaFile(entry.path());
		}
		// TODO: AssetFile이 meta 파일이 없을 시 만들어주기
		itr++;
	}
}

void Resources::MakeMetaFile(wstring assetPath)
{
	wstring metaPath = assetPath + L".meta";
	if (fs::exists(fs::path(metaPath)))
		return;
	wstring ext = fs::path(assetPath).extension();
	string sFileName;
	sFileName.assign(assetPath.begin() + assetPath.find_last_of(L"\\") + 1, assetPath.end());
	const char* fileName = sFileName.c_str();

	GUID guid;
	HRESULT result = CoCreateGuid(&guid);

	if (result != S_OK)
	{
		return;
	}

	// wstring인 Guid를 string(char)로 변환
	wchar_t wstringGuid[129];
	StringFromGUID2(guid, wstringGuid, 128);
	// Guid의 크기가 128바이트 인 걸 알고 있으니 아래 코드 필요 X
	// int wstrSize = WideCharToMultiByte(CP_ACP, 0, wstringGuid, -1, NULL, 0, NULL, NULL);
	// char* charGuid = new char[128];
	char charGuid[129];
	WideCharToMultiByte(CP_ACP, 0, wstringGuid, -1, charGuid, 129, 0, 0);


	// TODO: 테스트
	//
	//
	//

	MetaFormat metaFile;
	metaFile.guid = guid;

	string filePath;
	filePath.assign(assetPath.begin(), assetPath.begin() + assetPath.find_last_of(L"\\"));
	std::ofstream binOs(filePath + ".meta", std::ios::binary);


	//tinyxml2::XMLDocument newMetaFile;
	//tinyxml2::XMLNode* firstNode = newMetaFile.NewElement("Meta");
	//newMetaFile.InsertFirstChild(firstNode);
	//
	//tinyxml2::XMLElement* guidElement = newMetaFile.NewElement("GUID");
	//guidElement->SetValue(charGuid);

	//tinyxml2::XMLElement* importerElement = newMetaFile.NewElement("Importer");

	/*std::ofstream write;
	write.open(fileName);

	assert(write.is_open());

	write.write(stringGuid, stringGuid.size());*/

	if (ext == L".mat")
	{
		metaFile._ImporterType = ImporterType::DEFAULT;
	}
	else if (ext == L".fbx" || ext == L".mesh")
	{
		metaFile._ImporterType = ImporterType::DEFAULT;
	}
	else if (ext == L".hlsl" || ext == L".hlsli")
	{
		metaFile._ImporterType = ImporterType::DEFAULT;
	}
	else if (ext == L".h")
	{
		metaFile._ImporterType = ImporterType::MONO;
	}
	else if (ext != L".xml")
	{
		metaFile._ImporterType = ImporterType::DEFAULT;
	}

	write_typed_data(binOs, metaFile);
	binOs.close();

	/*tinyxml2::XMLError error = newMetaFile.SaveFile(fileName);*/
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

	objectID = _objectCount[static_cast<uint8>(objectType)]++; // 해당 오브젝트 맵의 사이즈를 기반으로 ID 부여
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
		// meta파일로부터 데이터를 읽어와 초기 설정 후 Add
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