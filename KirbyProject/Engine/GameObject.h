#pragma once


		class GameObject : public Object, public enable_shared_from_this<GameObject>
		{
		public:
			GameObject();
			virtual ~GameObject();

		public:
			void Awake();
			void Start();
			void Update();
			void LateUpdate();
			void FinalUpdate();

			shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

			shared_ptr<Transform> GetTransform();
			shared_ptr<Camera> GetCamera();

			void AddComponent(shared_ptr<Component> component);

		private:
			array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
			vector<shared_ptr<MonoBehaviour>> _scripts;
		};
