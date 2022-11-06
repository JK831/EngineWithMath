#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"


Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()
{
	Matrix3x3 matScale = Matrix3x3::CreateScale(_localScale);
	Matrix3x3 matRotation = Matrix3x3::CreateRotationZ(_localRotation);

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
	Matrix3x3 matWV = _matWorld * Camera::S_MatView;

}
