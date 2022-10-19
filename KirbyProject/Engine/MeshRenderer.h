#pragma once
#include "Component.h"


	class Material;
	namespace DD
	{
		class Mesh;
		

		class MeshRenderer : public Component
		{
		public:
			MeshRenderer();
			virtual ~MeshRenderer();

			void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
			void SetMaterial(shared_ptr<Material> material) { _material = material; }

			virtual void Update() override { Render(); }

			void Render();

		private:
			shared_ptr<Mesh> _mesh;
			shared_ptr<Material> _material;
		};
	}
