#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float	_speed = 100.f;
};

