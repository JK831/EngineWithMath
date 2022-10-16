#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

using namespace JK::DD;

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	shared_ptr<Matrix3x3> modelingMatrix = GetTransform()->GetMatrix();
	uint16 bufferIndex = _material->Update();
	_mesh->Render(modelingMatrix, bufferIndex);
}