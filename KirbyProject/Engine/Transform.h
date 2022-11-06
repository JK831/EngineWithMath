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
	const Vector2& GetLocalPosition() { return _localPosition; }
	float GetLocalRotation() { return _localRotation; }
	const Vector2& GetLocalScale() { return _localScale; }

	const Matrix3x3& GetLocalToWorldMatrix() { return _matWorld; }
	const Vector2& GetWorldPosition() { return _matWorld.Translation(); }

	Vector2 GetRight() { return _matWorld.Right(); }
	Vector2 GetUp() { return _matWorld.Up(); }
	Vector2 GetLook() { return -Vector3::UnitZ.ToVector2(); } // = 0 vector

	void SetLocalPosition(const Vector2& position) { _localPosition = position; }
	void SetLocalRotation(float rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vector2& scale) { _localScale = scale; }

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent 기준
	Vector2 _localPosition = {};
	float _localRotation = 0;
	Vector2 _localScale = Vector2(1.f, 1.f);

	Matrix3x3 _matLocal = {};
	Matrix3x3 _matWorld = {};

	weak_ptr<Transform> _parent;
};
