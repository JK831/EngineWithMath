#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"


MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Render()
{
	const Matrix3x3& finalMatrix = GetTransform()->GetLocalToWorldMatrix() * Camera::S_MatView;
	uint16 bufferIndex = _material->Update();
	_mesh->Render(finalMatrix, bufferIndex);
}