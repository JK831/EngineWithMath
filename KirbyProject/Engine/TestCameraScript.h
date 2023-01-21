#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
	REFLECT(TestCameraScript);
public:
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	virtual shared_ptr<MonoBehaviour> CreateInstance() override;
private:
	float	_speed = 100.f;
};

