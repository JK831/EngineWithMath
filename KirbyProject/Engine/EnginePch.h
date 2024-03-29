#pragma once

#include "MathHeaders.h" // string, vector, array included
#include "RendererHeaders.h"

#include <tchar.h>
#include <memory>

#include <list>
#include <map>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#pragma comment(lib, "MathModule.lib")
#pragma comment(lib, "RenderModule.lib")

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT,
	MATERIAL,
	MESH,
	SHADER,
	COMPONENT,
	SPRITE,
	TEXTURE,

	END,
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	// ...
	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

enum class MESH_TYPE : uint32
{
	NORMAL = 0,
	SKINNED,
};



enum class TextureFilterMode : UINT32
{
	Nearest = 0,
	Linear,
	Cubic
};

enum class TextureAddressMode : UINT32
{
	Repeat = 0,
	Clamp
};

enum
{
	MAIN_TEXTURE_NUM = 0
};

template<typename T>
wstring IntToBinary(T num)
{
	wstring bin;
	while (num > 0)
	{
		bin += to_wstring(num % 2);
		num /= 2;
	}
	bin = wstring.assign(bin.rbegin(), bin.rend());
	return bin;
}

// 바이너리 파일 save, load를 위한 함수
template<typename T>
std::ostream& write_typed_data(std::ostream& stream, const T& value) {
	return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}
template<typename T>
std::istream& read_typed_data(std::istream& stream, T& value) {
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}

#define DECLARE_SINGLE(type)	\
private:						\
	type() {}					\
	~type() {}					\
public:							\
	static type* GetInstance()	\
	{							\
		static type instance;	\
		return &instance;		\
	}							\

#define GET_SINGLE(type) type::GetInstance()

#define CMD_LIST		GEngine->GetRenderQueue()

#define INPUT		GET_SINGLE(Input)
#define DELTA_TIME	GET_SINGLE(Timer)->GetDeltaTime()

#define REFLECT(type)													\
public:																	\
	type()																\
	{																	\
		MonoBehaviour::RegisterScript(L#type, type::_staticObject);	\
	}																	\
	static type _staticObject;													\

extern unique_ptr<class Engine> GEngine;
