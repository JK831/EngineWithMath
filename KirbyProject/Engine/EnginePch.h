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

struct Vertex
{
	Vertex() {}

	Vertex(Vector2 p, Vector2 u, Vector2 n, Vector2 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vector2 pos;
	Vector2 uv;
	Vector2 normal;
	Vector2 tangent;
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

extern unique_ptr<class Engine> GEngine;
