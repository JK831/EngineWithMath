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

	Matrix4x4 _matView = {};
	Matrix4x4 _matProjection = {};

public:
	// TEMP
	static Matrix4x4 S_MatView;
	static Matrix4x4 S_MatProjection;
};