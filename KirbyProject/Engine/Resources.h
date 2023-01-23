#pragma once

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MonoBehaviour.h"

class Resources
{
	DECLARE_SINGLE(Resources);

public:
	void Init(wstring assetPath);
	void ParsingGuid(wstring assetPath);
	void ParsingGuidFromMeta(wstring assetPath);
	void MakeMeta(wstring assetPath);
	void MakeMetaFile(wstring assetPath);
	OBJECT_TYPE GetObjectTypeByExt(wstring& filePath);
	wstring GetPathByKey(wstring key);
	template<typename T>
	shared_ptr<T> LoadRegisteredAsset(wstring key);

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& Key);

	template<typename T>
	OBJECT_TYPE GetObjectType();
	const wstring& GetPathByGuid(string& guid);

	shared_ptr<Mesh> LoadRectangleMesh();
	shared_ptr<Mesh> LoadCircleMesh();

private:
	void ParseAssetFiles(fs::path InPath);

private:
	uint8 _fileIDSize = sizeof(uint16) * 8;

	uint16 _objectCount[OBJECT_TYPE_COUNT] = { 0, };
	std::map<wstring, wstring> _pathIDMap;
	std::map<wstring, wstring> _IDPathMap;

	using KeyObjMap = std::map<wstring/*key*/, shared_ptr<Object>>;
	array<KeyObjMap, OBJECT_TYPE_COUNT> _resources;

	std::map<string, wstring> _GuidPathMap;
};

// Possibility to generate exception
inline wstring Resources::GetPathByKey(wstring key)
{
	if (_IDPathMap.find(key) == _IDPathMap.end())
		return L"Wrong Key";
	return _IDPathMap[key];
}

template<typename T>
shared_ptr<T> Resources::LoadRegisteredAsset(wstring key)
{
	for (int i = 0; i < OBJECT_TYPE_COUNT; i++)
	{
		KeyObjMap& keyObjMap = _resources[i];
		if (keyObjMap.find(key) != keyObjMap.end())
			return static_pointer_cast<T>(keyObjMap[key]);
	}
	return nullptr;
}

// Key를 부여하면서 Load하고 싶을 때
template<typename T>
inline shared_ptr<T> Resources::Load(const wstring& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
bool Resources::Add(const wstring& key, shared_ptr<T> object)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;

	return true;
}

template<typename T>
shared_ptr<T> Resources::Get(const wstring& key)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
FORCEINLINE OBJECT_TYPE Resources::GetObjectType()
{
	if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::GAMEOBJECT;
	else if (std::is_same_v<T, Material>)
		return OBJECT_TYPE::MATERIAL;
	else if (std::is_same_v<T, Mesh>)
		return OBJECT_TYPE::MESH;
	else if (std::is_same_v<T, Shader>)
		return OBJECT_TYPE::SHADER;
	else if (std::is_same_v<T, Texture>)
		return OBJECT_TYPE::TEXTURE;
	else if (std::is_convertible_v<T, Component>)
		return OBJECT_TYPE::COMPONENT;
	else
		return OBJECT_TYPE::NONE;
}

// A error can occur when a header file defines a function that isn't inline.
FORCEINLINE const wstring& Resources::GetPathByGuid(string& guid)
{
	return _GuidPathMap[guid];
}