#pragma once





		class Mesh : public Object
		{
		public:
			Mesh();
			virtual ~Mesh();

			void Init(const ::std::vector<Vertex>& inVertices, const ::std::vector<size_t>& inIndices);
			void Render(::std::shared_ptr<Matrix3x3> InMatrix, uint16 InBufferIndex);

			void CreateVertexBuffer(const ::std::vector<Vertex>& buffer);
			void CreateIndexBuffer(const vector<uint32>& buffer);

			// 속성 관련 함수
			void SetMeshType(const MESH_TYPE& _InMeshType) { _MeshType = _InMeshType; }
			FORCEINLINE bool HasColor() const { return _Colors.size() > 0; }
			FORCEINLINE bool HasUV() const { return _UVs.size() > 0; }
			vector<Vertex>& GetVertices() { return _vertexBuffer; }
			const ::std::vector<Vertex>& GetVertices() const { return _vertexBuffer; }
			::std::vector<uint32>& GetIndices() { return _indexBuffer; }
			const ::std::vector<uint32>& GetIndices() const { return _indexBuffer; }
			::std::vector<LinearColor>& GetColors() { return _Colors; }
			const ::std::vector<LinearColor>& GetColors() const { return _Colors; }
			::std::vector<Vector2>& GetUVs() { return _UVs; }
			const ::std::vector<Vector2>& GetUVs() const { return _UVs; }

			// 바운딩 볼륨 관련 함수
			void CalculateBounds();
			const Circle& GetCircleBound() const { return _CircleBound; }
			const Rectangle& GetRectBound() const { return _RectBound; }

		private:
			::std::vector<Vertex> _vertexBuffer;
			uint32 _vertexCount = 0;
			::std::vector<uint32> _indexBuffer;
			uint32 _indexCount = 0;
			::std::vector<LinearColor> _Colors;
			::std::vector<Vector2> _UVs;

		private:
			MESH_TYPE _MeshType = MESH_TYPE::NORMAL;
			Circle _CircleBound;
			Rectangle _RectBound;
		};


