#pragma once


	struct Matrix4x4
	{
	public:
		// ������ 
		FORCEINLINE constexpr Matrix4x4() = default;
		FORCEINLINE explicit constexpr Matrix4x4(const Vector4& InRaw0, const Vector4& InRaw1, const Vector4& InRaw2, const Vector4& InCol3) { Raws = { InRaw0, InRaw1, InRaw2, InCol3 }; }

		// ������ 
		FORCEINLINE const Vector4& operator[](BYTE InIndex) const;
		FORCEINLINE Vector4& operator[](BYTE InIndex);
		FORCEINLINE constexpr Matrix4x4 operator*(float InScalar) const;
		FORCEINLINE constexpr Matrix4x4 operator*(const Matrix4x4& InMatrix) const;
		FORCEINLINE constexpr Vector4 operator*(const Vector4& InVector) const;
		FORCEINLINE friend Vector4 operator*=(Vector4& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}
		FORCEINLINE constexpr Vector3 operator*(const Vector3& InVector) const;
		FORCEINLINE friend Vector3 operator*=(Vector3& InVector, const Matrix4x4& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}

		// ����Լ� 
		FORCEINLINE Matrix3x3 ToMatrix3x3() const;
		FORCEINLINE void SetIdentity();
		FORCEINLINE constexpr Matrix4x4 Transpose() const;

		std::vector<std::string> ToStrings() const;

		// ����������� 
		static const Matrix4x4 Identity;
		static constexpr BYTE Rank = 4;

		// ������� 
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
		Matrix4x4 transposedMatrix = Transpose();
		return Matrix4x4(
			Vector4(transposedMatrix[0].Dot(InMatrix[0]), transposedMatrix[1].Dot(InMatrix[0]), transposedMatrix[2].Dot(InMatrix[0]), transposedMatrix[3].Dot(InMatrix[0])),
			Vector4(transposedMatrix[0].Dot(InMatrix[1]), transposedMatrix[1].Dot(InMatrix[1]), transposedMatrix[2].Dot(InMatrix[1]), transposedMatrix[3].Dot(InMatrix[1])),
			Vector4(transposedMatrix[0].Dot(InMatrix[2]), transposedMatrix[1].Dot(InMatrix[2]), transposedMatrix[2].Dot(InMatrix[2]), transposedMatrix[3].Dot(InMatrix[2])),
			Vector4(transposedMatrix[0].Dot(InMatrix[3]), transposedMatrix[1].Dot(InMatrix[3]), transposedMatrix[2].Dot(InMatrix[3]), transposedMatrix[3].Dot(InMatrix[3]))
		);
	}

	FORCEINLINE constexpr Vector4 Matrix4x4::operator*(const Vector4& InVector) const
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
		Vector4 result = *this * v4;

		return Vector3(result.X, result.Y, result.Z);
	}

	FORCEINLINE Matrix3x3 Matrix4x4::ToMatrix3x3() const
	{
		return Matrix3x3(Raws[0].ToVector3(), Raws[1].ToVector3(), Raws[2].ToVector3());
	}

