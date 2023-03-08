#pragma once

struct Vertex
{
	Vertex() {}

	Vertex(Vector3 p, Vector2 u, Vector3 n, Vector3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

namespace DD
{
	struct Vertex2D
	{
	public:
		constexpr Vertex2D() = default;
		constexpr Vertex2D(const Vector2& InPosition) : Position(InPosition) { }
		constexpr Vertex2D(const Vector2& InPosition, const LinearColor& InColor) : Position(InPosition), Color(InColor) { }
		constexpr Vertex2D(const Vector2& InPosition, const LinearColor& InColor, const Vector2& InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

		constexpr Vertex2D operator*(float InScalar) const
		{
			return Vertex2D(
				Position * InScalar,
				Color * InScalar,
				UV * InScalar
			);
		}

		constexpr Vertex2D operator+(const Vertex2D& InVector) const
		{
			return Vertex2D(
				Position + InVector.Position,
				Color + InVector.Color,
				UV + InVector.UV
			);
		}

		Vector2 Position;
		LinearColor Color;
		Vector2 UV;
	};
}

namespace DDD
{

	struct Vertex3D
	{
	public:
		constexpr Vertex3D() = default;
		constexpr Vertex3D(const Vector4& InPosition) : Position(InPosition) { }
		constexpr Vertex3D(const Vector4& InPosition, const LinearColor& InColor) : Position(InPosition), Color(InColor) { }
		constexpr Vertex3D(const Vector4& InPosition, const LinearColor& InColor, const Vector2& InUV) : Position(InPosition), Color(InColor), UV(InUV) { }

		constexpr Vertex3D operator*(float InScalar) const
		{
			return Vertex3D(
				Position * InScalar,
				Color * InScalar,
				UV * InScalar
			);
		}

		constexpr Vertex3D operator+(const Vertex3D& InVector) const
		{
			return Vertex3D(
				Position + InVector.Position,
				Color + InVector.Color,
				UV + InVector.UV
			);
		}

		Vector4 Position;
		LinearColor Color;
		Vector2 UV;
	};

}