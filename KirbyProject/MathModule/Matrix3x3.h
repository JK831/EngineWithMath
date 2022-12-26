#pragma once


	class Matrix3x3
	{
	public:
		// 생성자 
		FORCEINLINE constexpr Matrix3x3() = default;
		FORCEINLINE explicit constexpr Matrix3x3(const Vector3& InRaw0, const Vector3& InRaw1, const Vector3& InRaw2) { Raws = { InRaw0, InRaw1, InRaw2 }; }

		// 연산자 
		FORCEINLINE const Vector3& operator[](BYTE InIndex) const;
		FORCEINLINE Vector3& operator[](BYTE InIndex);

		FORCEINLINE Matrix3x3 operator*(float InScalar) const;
		FORCEINLINE Matrix3x3 operator*(const Matrix3x3& InMatrix) const;
		FORCEINLINE Matrix3x3& operator*=(const Matrix3x3& InMatrix);
		FORCEINLINE Vector3 operator*(const Vector3& InVector) const;
		
		FORCEINLINE friend Vector2 operator*(Vector2& InVector2, const Matrix3x3& InMatrix)
		{
			return InVector2 * InMatrix.ToMatrix2x2();
		}

		FORCEINLINE friend Vector3 operator*(Vector3& InVector3, const Matrix3x3& InMatrix)
		{
			Matrix3x3 transposedMatrix = InMatrix.Transpose();
			return Vector3(InVector3.X * transposedMatrix.Raws[0].X + InVector3.Y * transposedMatrix.Raws[0].Y + InVector3.Z * transposedMatrix.Raws[0].Z,
				InVector3.X * transposedMatrix.Raws[1].X + InVector3.Y * transposedMatrix.Raws[1].Y + InVector3.Z * transposedMatrix.Raws[1].Z,
				InVector3.X * transposedMatrix.Raws[2].X + InVector3.Y * transposedMatrix.Raws[2].Y + InVector3.Z * transposedMatrix.Raws[2].Z);
		}

		FORCEINLINE friend Vector2& operator*=(Vector2& InVector, const Matrix3x3& InMatrix)
		{
			InVector = InVector * InMatrix.ToMatrix2x2();
			return InVector;
		}

		FORCEINLINE friend Vector3& operator*=(Vector3& InVector, const Matrix3x3& InMatrix)
		{
			InVector = InVector * InMatrix;
			return InVector;
		}

		// 멤버함수 
		FORCEINLINE Matrix2x2 ToMatrix2x2() const;
		FORCEINLINE void SetIdentity();
		FORCEINLINE Matrix3x3 Transpose() const;
		FORCEINLINE Matrix3x3 Invert() const
		{
			float d = Raws[0][0] * (Raws[1][1] * Raws[2][2] - Raws[2][1] * Raws[1][2]) - Raws[0][1] * (Raws[1][0] * Raws[2][2] - Raws[1][2] * Raws[2][0]) + Raws[0][2] * (Raws[1][0] * Raws[2][1] - Raws[1][1] * Raws[2][0]);
			if (d != 0.)
			{
				Matrix3x3 invertedMatrix;
				float id = 1 / d;
				invertedMatrix.Raws[0][0] = (Raws[1][1] * Raws[2][2] - Raws[2][1] * Raws[1][2]) * id;
				invertedMatrix.Raws[0][1] = (Raws[0][2] * Raws[2][1] - Raws[0][1] * Raws[2][2]) * id;
				invertedMatrix.Raws[0][2] = (Raws[0][1] * Raws[1][2] - Raws[0][2] * Raws[1][1]) * id;
				invertedMatrix.Raws[1][0] = (Raws[1][2] * Raws[2][0] - Raws[1][0] * Raws[2][2]) * id;
				invertedMatrix.Raws[1][1] = (Raws[0][0] * Raws[2][2] - Raws[0][2] * Raws[2][0]) * id;
				invertedMatrix.Raws[1][2] = (Raws[1][0] * Raws[0][2] - Raws[0][0] * Raws[1][2]) * id;
				invertedMatrix.Raws[2][0] = (Raws[1][0] * Raws[2][1] - Raws[2][0] * Raws[1][1]) * id;
				invertedMatrix.Raws[2][1] = (Raws[2][0] * Raws[0][1] - Raws[0][0] * Raws[2][1]) * id;
				invertedMatrix.Raws[2][2] = (Raws[0][0] * Raws[1][1] - Raws[1][0] * Raws[0][1]) * id;

				return invertedMatrix;
			}
		}

		FORCEINLINE Vector2 Translation() const noexcept;
		FORCEINLINE void Translation(Vector2& InVector) { Raws[2].X = InVector.X; Raws[2].Y = InVector.Y; }
		FORCEINLINE Vector2 Up() const noexcept { return Vector2(Raws[1].X, Raws[1].Y); };
		FORCEINLINE void Up(Vector2& InVector) { Raws[1].X = InVector.X; Raws[1].Y = InVector.Y; }
		FORCEINLINE Vector2 Right() const noexcept { return Vector2(Raws[0].X, Raws[0].Y); };
		FORCEINLINE void Right(Vector2& InVector) { Raws[0].X = InVector.X; Raws[0].Y = InVector.Y; };
		


		std::vector<std::string> ToStrings() const;

		static Matrix3x3 CreateRotationZ(float InRadian);
		static Matrix3x3 CreateTranslation(Vector2 InPosition);
		static Matrix3x3 CreateScale(Vector2 InScale) { return Matrix3x3(Vector3(InScale.X, 0, 0), Vector3(0, InScale.Y, 0), Vector3::UnitZ); }

		// 정적멤버변수 
		static const Matrix3x3 Identity;
		static constexpr BYTE Rank = 3;

		// 멤버변수 
		std::array<Vector3, Rank> Raws = { Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ };
	};

	FORCEINLINE void Matrix3x3::SetIdentity()
	{
		*this = Matrix3x3::Identity;
	}

	FORCEINLINE Matrix3x3 Matrix3x3::Transpose() const
	{
		return Matrix3x3(
			Vector3(Raws[0].X, Raws[1].X, Raws[2].X),
			Vector3(Raws[0].Y, Raws[1].Y, Raws[2].Y),
			Vector3(Raws[0].Z, Raws[1].Z, Raws[2].Z)
		);
	}

	FORCEINLINE Vector2 Matrix3x3::Translation() const noexcept
	{
		return Vector2(Raws[2].X, Raws[2].Y);
	}

	FORCEINLINE const Vector3& Matrix3x3::operator[](BYTE InIndex) const
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE Vector3& Matrix3x3::operator[](BYTE InIndex)
	{
		assert(InIndex < Rank);
		return Raws[InIndex];
	}

	FORCEINLINE Matrix3x3 Matrix3x3::operator*(float InScalar) const
	{
		return Matrix3x3(
			Raws[0] * InScalar,
			Raws[1] * InScalar,
			Raws[2] * InScalar
		);
	}

	FORCEINLINE Matrix3x3 Matrix3x3::operator*(const Matrix3x3& InMatrix) const
	{
		Matrix3x3 transposedMatrix = InMatrix.Transpose();
		return Matrix3x3(
			Vector3(Raws[0].Dot(transposedMatrix[0]), Raws[0].Dot(transposedMatrix[1]), Raws[0].Dot(transposedMatrix[2])),
			Vector3(Raws[1].Dot(transposedMatrix[0]), Raws[1].Dot(transposedMatrix[1]), Raws[1].Dot(transposedMatrix[2])),
			Vector3(Raws[2].Dot(transposedMatrix[0]), Raws[2].Dot(transposedMatrix[1]), Raws[2].Dot(transposedMatrix[2]))
		);

	}

	FORCEINLINE Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& InMatrix)
	{
		Matrix3x3 transposedMatrix = InMatrix.Transpose();
		Raws[0] = Vector3(Raws[0].Dot(transposedMatrix[0]), Raws[0].Dot(transposedMatrix[1]), Raws[0].Dot(transposedMatrix[2]));
		Raws[1] = Vector3(Raws[1].Dot(transposedMatrix[0]), Raws[1].Dot(transposedMatrix[1]), Raws[1].Dot(transposedMatrix[2]));
		Raws[2] = Vector3(Raws[2].Dot(transposedMatrix[0]), Raws[2].Dot(transposedMatrix[1]), Raws[2].Dot(transposedMatrix[2]));

		return *this;
	}

	FORCEINLINE Vector3 Matrix3x3::operator*(const Vector3& InVector) const
	{
		return Vector3(
			Raws[0].Dot(InVector),
			Raws[1].Dot(InVector),
			Raws[2].Dot(InVector)
		);
	}


	FORCEINLINE Matrix2x2 Matrix3x3::ToMatrix2x2() const
	{
		return Matrix2x2(Raws[0].ToVector2(), Raws[1].ToVector2());
	}
