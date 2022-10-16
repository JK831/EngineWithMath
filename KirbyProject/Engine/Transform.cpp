#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

using namespace JK::DD;

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()
{
	Matrix3x3 matScale = Matrix3x3::CreateScale(_localScale);
	Matrix3x3 matRotation = Matrix3x3::CreateRotationX(_localRotation.x);
	matRotation *= Matrix3x3::CreateRotationY(_localRotation.y);
	matRotation *= Matrix3x3::CreateRotationZ(_localRotation.z);
	Matrix3x3 matTranslation = Matrix3x3::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;
	_matWorld = _matLocal;

	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

void Transform::PushData()
{
	Matrix3x3 matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&matWVP, sizeof(matWVP));
}
