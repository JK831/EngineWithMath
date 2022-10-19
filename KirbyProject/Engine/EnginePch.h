#pragma once

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "MathHeaders.h"
#include "RendererHeaders.h"

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

	Vertex(JK::Vector3 p, JK::Vector2 u, JK::Vector3 n, JK::Vector3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	JK::Vector3 pos;
	JK::Vector2 uv;
	JK::Vector3 normal;
	JK::Vector3 tangent;
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

struct WindowInfo
{
	HWND hwnd; // 출력 윈도우
	int32 width; // 너비
	int32 height; // 높이
	bool windowed; // 창모드 or FUllScreen
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

#define CMD_LIST		GEngine->GetRenderQueue

#define INPUT		GET_SINGLE(Input)
#define DELTA_TIME	GET_SINGLE(Timer)->GetDeltaTime()

extern unique_ptr<class Engine> GEngine;
