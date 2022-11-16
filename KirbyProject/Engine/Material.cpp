#include "pch.h"
#include "Material.h"
#include "Engine.h"
#include "RenderQueue.h"

	Material::Material() : Object(OBJECT_TYPE::MATERIAL)
	{
		_indexNum = 0;
	}

	Material::~Material()
	{

	}

	uint16 Material::Update()
	{
		return GEngine->GetRenderQueue()->PushMaterial(_indexNum, *this);
	}