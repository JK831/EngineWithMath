#pragma once

#include "Component.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};


class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = Math::PI / 4.f;
	float _scale = 1.f;

	Matrix3x3 _matView = {};
	Matrix3x3 _matProjection = {};

public:
	// TEMP
	static Matrix3x3 S_MatView;
	static Matrix3x3 S_MatProjection;
};