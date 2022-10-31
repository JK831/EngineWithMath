#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"

TestCameraScript::TestCameraScript()
{

}

TestCameraScript::~TestCameraScript()
{

}

void TestCameraScript::LateUpdate()
{
	Vector2 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
		pos += GetTransform()->GetUp() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::LEFT))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vector2 rotation = GetTransform()->GetLocalRotation();
		rotation.X += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vector2 rotation = GetTransform()->GetLocalRotation();
		rotation.X -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	GetTransform()->SetLocalPosition(pos);
}