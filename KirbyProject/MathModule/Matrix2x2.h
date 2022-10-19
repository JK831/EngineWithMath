#pragma once


	struct Matrix2x2
	{
	public:
		// 생성자 
		FORCEINLINE constexpr Matrix2x2() = default;
		FORCEINLINE explicit constexpr Matrix2x2(const Vector2& InRaw0, const Vector2& InRaw1) { 
			Raws = { InRaw0, InRaw1 }; }

		// 연산자 
		FORCEINLINE const Vector2& operator[](BYTE InIndex) const;
		FORCEINLINE Vector2& operator[](BYTE InIndex);
		FORCEINLINE Matrix2x2 operator*(float InScalar) const;
		FORCEINLINE Matrix2x2 operator*(const Matrix2x2& InMatrix) const;
		FORCEINLINE Vector2 operator*(const Vector2& InVector) const;

		FORCEINLINE friend Vector2 operator*=(Vector2& InVector, const Matrix2x2& InMatrix)
		{
			InVector = InMatrix * InVector;
			return InVector;
		}

		// 멤버함수 
		FORCEINLINE void SetIdentity();
		FORCEINLINE Matrix2x2 Transpose() const;

		std::vector<std::string> ToStrings() const;

		// 정적멤버변수 
		static const Matrix2x2 Identity;
		static constexpr BYTE Rank = 2;

		// 멤버변수 
		std::array<Vector2, Rank> Raws = { Vector2::UnitX, Vector2::UnitY };
	};

	FORCEINLINE void Matrix2x2::SetIdentity()
	{
		*this = Matrix2x2::Identity;
	}

	FORCEINLINE Matrix2x2 Matrix2x2::Transpose() const
	{
		return Matrix2x2(
			Vector2(Raws[0].X, Raws[1].X),
			Vector2(Raws[0].Y, Raws[1].Y)
		);
	}

	FORCEINLINE const Vector2& Matrix2x2::operator[](BYTE InIndex) const
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE Vector2& Matrix2x2::operator[](BYTE InIndex)
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE Matrix2x2 Matrix2x2::operator*(float InScalar) const
	{
		return Matrix2x2(
			Raws[0] * InScalar,
			Raws[1] * InScalar
		);
	}

	FORCEINLINE Matrix2x2 Matrix2x2::operator*(const Matrix2x2& InMatrix) const
	{
		Matrix2x2 transposedMatrix = InMatrix.Transpose();
		return Matrix2x2(
			Vector2(Raws[0].Dot(transposedMatrix[0]), Raws[1].Dot(transposedMatrix[0])),
			Vector2(Raws[0].Dot(transposedMatrix[1]), Raws[1].Dot(transposedMatrix[1]))
		);
	}

	FORCEINLINE Vector2 Matrix2x2::operator*(const Vector2& InVector) const
	{
		return Vector2(
			Raws[0].Dot(InVector),
			Raws[1].Dot(InVector)
		);
	}

