#pragma once


	class Matrix4x4
	{
	public:
		// 생성자 
		FORCEINLINE constexpr Matrix4x4() = default;
		FORCEINLINE explicit constexpr Matrix4x4(const Vector4& InRaw0, const Vector4& InRaw1, const Vector4& InRaw2, const Vector4& InCol3) { Raws = { InRaw0, InRaw1, InRaw2, InCol3 }; }

		// 연산자 
		FORCEINLINE const Vector4& operator[](BYTE InIndex) const;
		FORCEINLINE Vector4& operator[](BYTE InIndex);
		FORCEINLINE constexpr Matrix4x4 operator*(float InScalar) const;
		FORCEINLINE constexpr Matrix4x4 operator*(const Matrix4x4& InMatrix) const;
		// FORCEINLINE constexpr Vector4 operator*(const Vector4& InVector) const;
		FORCEINLINE friend Vector2 operator*(Vector2& InVector2, const Matrix4x4& InMatrix)
		{
			return InVector2 * InMatrix.ToMatrix3x3();
		}

		FORCEINLINE friend Vector3 operator*(Vector3& InVector3, const Matrix4x4& InMatrix)
		{
			Matrix3x3 transposedMatrix = InMatrix.ToMatrix3x3().Transpose();
			return Vector3(
				transposedMatrix[0].Dot(InVector3),
				transposedMatrix[1].Dot(InVector3),
				transposedMatrix[2].Dot(InVector3)
			);
		}

		FORCEINLINE friend Vector4 operator*(Vector4& InVector4, const Matrix4x4& InMatrix)
		{
			Matrix4x4 transposedMatrix = InMatrix.Transpose();
			return Vector4(
				transposedMatrix[0].Dot(InVector4),
				transposedMatrix[1].Dot(InVector4),
				transposedMatrix[2].Dot(InVector4),
				transposedMatrix[3].Dot(InVector4)
			);
		}

		// 벡터와의 연산을 위한 operator overloading
		FORCEINLINE friend Vector2& operator*=(Vector2& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InVector * InMatrix.ToMatrix3x3();
			return InVector;
		}

		FORCEINLINE friend Vector3& operator*=(Vector3& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InVector * InMatrix.ToMatrix3x3();
			return InVector;
		}
		FORCEINLINE friend Vector4& operator*=(Vector4& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InVector * InMatrix;
			return InVector;
		}
		//FORCEINLINE constexpr Vector3 operator*(const Vector3& InVector) const;


		// 멤버함수 
		FORCEINLINE Matrix3x3 ToMatrix3x3() const;
		FORCEINLINE void SetIdentity();
		FORCEINLINE constexpr Matrix4x4 Transpose() const;

		std::vector<std::string> ToStrings() const;

		// 정적멤버변수 
		static const Matrix4x4 Identity;
		static constexpr BYTE Rank = 4;

		// 멤버변수 
		std::array<Vector4, Rank> Raws = { Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4::UnitW };
	};

	FORCEINLINE void Matrix4x4::SetIdentity()
	{
		*this = Matrix4x4::Identity;
	}

	FORCEINLINE constexpr Matrix4x4 Matrix4x4::Transpose() const
	{
		return Matrix4x4(
			Vector4(Raws[0].X, Raws[1].X, Raws[2].X, Raws[3].X),
			Vector4(Raws[0].Y, Raws[1].Y, Raws[2].Y, Raws[3].Y),
			Vector4(Raws[0].Z, Raws[1].Z, Raws[2].Z, Raws[3].Z),
			Vector4(Raws[0].W, Raws[1].W, Raws[2].W, Raws[3].W)
		);
	}

	FORCEINLINE const Vector4& Matrix4x4::operator[](BYTE InIndex) const
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE Vector4& Matrix4x4::operator[](BYTE InIndex)
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE constexpr Matrix4x4 Matrix4x4::operator*(float InScalar) const
	{
		return Matrix4x4(
			Raws[0] * InScalar,
			Raws[1] * InScalar,
			Raws[2] * InScalar,
			Raws[3] * InScalar
		);
	}

	FORCEINLINE constexpr Matrix4x4 Matrix4x4::operator*(const Matrix4x4& InMatrix) const
	{
		Matrix4x4 transposedMatrix = InMatrix.Transpose();
		return Matrix4x4(
			Vector4(Raws[0].Dot(transposedMatrix[0]), Raws[0].Dot(transposedMatrix[1]), Raws[0].Dot(transposedMatrix[2]), Raws[0].Dot(transposedMatrix[3])),
			Vector4(Raws[1].Dot(transposedMatrix[0]), Raws[1].Dot(transposedMatrix[1]), Raws[1].Dot(transposedMatrix[2]), Raws[1].Dot(transposedMatrix[3])),
			Vector4(Raws[2].Dot(transposedMatrix[0]), Raws[2].Dot(transposedMatrix[1]), Raws[2].Dot(transposedMatrix[2]), Raws[2].Dot(transposedMatrix[3])),
			Vector4(Raws[3].Dot(transposedMatrix[0]), Raws[2].Dot(transposedMatrix[1]), Raws[3].Dot(transposedMatrix[2]), Raws[3].Dot(transposedMatrix[3]))
		);
	}

	/*FORCEINLINE constexpr Vector4 Matrix4x4::operator*(const Vector4& InVector) const
	{
		Matrix4x4 transposedMatrix = Transpose();
		return Vector4(
			transposedMatrix[0].Dot(InVector),
			transposedMatrix[1].Dot(InVector),
			transposedMatrix[2].Dot(InVector),
			transposedMatrix[3].Dot(InVector)
		);
	}

	FORCEINLINE constexpr Vector3 Matrix4x4::operator*(const Vector3& InVector) const
	{
		Vector4 v4(InVector);
		v4 *= *this;

		return Vector3(v4.X, v4.Y, v4.Z);
	}*/

	FORCEINLINE Matrix3x3 Matrix4x4::ToMatrix3x3() const
	{
		return Matrix3x3(Raws[0].ToVector3(), Raws[1].ToVector3(), Raws[2].ToVector3());
	}

