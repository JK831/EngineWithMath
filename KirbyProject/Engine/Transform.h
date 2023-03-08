#pragma once
#include "Component.h"


class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent 기준
	const Vector3& GetLocalPosition() { return _localPosition; }
	float GetLocalRotation() { return _localRotation; }
	const Vector3& GetLocalScale() { return _localScale; }

	const Matrix3x3& GetLocalToWorldMatrix() { return _matWorld; }
	const Vector3& GetWorldPosition() { return _matWorld.Translation(); }

	Vector3 GetRight() { return _matWorld.Right(); }
	Vector3 GetUp() { return _matWorld.Up(); }
	Vector3 GetLook() { return -Vector3::UnitZ.ToVector2(); } // = 0 vector

	void SetLocalPosition(const Vector3& position) { _localPosition = position; }
	void SetLocalRotation(float rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vector3& scale) { _localScale = scale; }

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent 기준
	Vector3 _localPosition = Vector3(0.f, 0.f, 0.f);
	float _localRotation = 0;
	Vector3 _localScale = Vector3(1.f, 1.f, 1.f);

	Matrix3x3 _matLocal = {};
	Matrix3x3 _matWorld = {};

	weak_ptr<Transform> _parent;
};
