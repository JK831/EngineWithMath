#include "pch.h"
#include "Material.h"

namespace JK
{
	Material::Material() : Object(OBJECT_TYPE::MATERIAL)
	{

	}

	Material::~Material()
	{

	}

	uint16 Material::Update()
	{
		return GEngine->GetRenderQueue()->PushMaterial();
	}
}