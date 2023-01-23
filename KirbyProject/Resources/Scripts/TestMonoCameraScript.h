#pragma once
#include "MonoBehaviour.h"

class TestMonoCameraScript : public MonoBehaviour
{
	REFLECT(TestMonoCameraScript);
public:
	virtual ~TestMonoCameraScript();

	virtual void LateUpdate() override;

	virtual shared_ptr<MonoBehaviour> CreateInstance() override;
private:
	float	_speed = 100.f;
};

