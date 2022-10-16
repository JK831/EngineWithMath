#pragma once
#include "Component.h"

namespace JK
{
	namespace DD
	{
		class Transform : public Component
		{
		public:
			Transform();
			virtual ~Transform();

			virtual void FinalUpdate() override;
			void PushData();

		public:
			// Parent ����
			const Vector2& GetLocalPosition() { return _localPosition; }
			const Vector2& GetLocalRotation() { return _localRotation; }
			const Vector2& GetLocalScale() { return _localScale; }

			const Matrix3x3& GetLocalToWorldMatrix() { return _matWorld; }
			const Vector2& GetWorldPosition() { return _matWorld.Translation(); }

			Vector2 GetRight() { return _matWorld.Right(); }
			Vector2 GetUp() { return _matWorld.Up(); }
			Vector2 GetLook() { return _matWorld.Backward(); }

			void SetLocalPosition(const Vector2& position) { _localPosition = position; }
			void SetLocalRotation(const Vector2& rotation) { _localRotation = rotation; }
			void SetLocalScale(const Vector2& scale) { _localScale = scale; }

		public:
			void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
			weak_ptr<Transform> GetParent() { return _parent; }

		private:
			// Parent ����
			Vec3 _localPosition = {};
			Vec3 _localRotation = {};
			Vec3 _localScale = { 1.f, 1.f, 1.f };

			Matrix3x3 _matLocal = {};
			Matrix3x3 _matWorld = {};

			weak_ptr<Transform> _parent;
		};
	}
}