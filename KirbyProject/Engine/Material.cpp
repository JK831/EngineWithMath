#include "pch.h"
#include "Material.h"


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