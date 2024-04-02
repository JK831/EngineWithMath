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
			return Vector3(
				transposedMatrix[0].Dot(InVector3),
				transposedMatrix[1].Dot(InVector3),
				transposedMatrix[2].Dot(InVector3)
			);
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
			/*-------------------------------------------------------------------
				가우스-요르단 소거법을 사용하여 행렬의 역행렬을 구한다.
				AX = I -> A를 기본 행 연산을 사용하여 I로 만드는 과정에서 사용된 연산을
				I에 그대로 취해줄 시 X (A의 역행렬) 구할 수 있다.
			 --------------------------------------------------------------------*/

			 /*-------------------------------------------------------------------
				 LU 분해를 사용하여 행렬을 상삼각, 하삼각 행렬의 곱으로 만들어주고
				 determinant를 구하여 행렬식을 구할 수 있다.
				 A = LU -> A = | a b c || g 0 0 |
							   | 0 d e || h i 0 |
							   | 0 0 f || j k l |
			  --------------------------------------------------------------------*/
			  // -> 대상 행렬을 상삼각 행렬로 만든 행렬이 L
			  //	  단위 행렬 I에 상삼각 행렬로 만들어 줄 때 사용된 연산의 역을 더해준 값 = U.
			  // 추가: pLU 분해를 고려하지 않아도 되는 이유 => 어떤 GameObject의 World matrix (transform) = SRT
			  // S는 Scale 행렬로서 3차원 공간에서 부피가 있는 오브젝트의 경우 대각성분이 0이 될 수 없다.

			Matrix3x3 L(Raws[0], Raws[1], Raws[2]);
			Matrix3x3 U; // AX = I 에서 X가 될 행렬, (초기 값 = I)
			U.SetIdentity();

			// 1열에 대한 기본 행 연산 -> Raw[0][0] (그대로) Raw[1][0] = 0 Raw[2][0] = 0 의 형태가 된다.
			float record = Raws[1][0] / Raws[0][0];
			L.Raws[1] -= Raws[0] * record;
			U.Raws[1][0] -= record; // 기록된 연산 취해준다.

			record = Raws[2][0] / Raws[0][0];
			L.Raws[2] -= Raws[0] * record;
			U.Raws[2][0] -= record;

			// 2열에 대한 기본 행 연산 -> Raw[0][1] Raw[1][1] Raw[2][1] = 0의 형태가 된다.
			record = Raws[2][1] / Raws[1][1];
			L.Raws[2] -= Raws[1] * record;
			U.Raws[2][1] -= record;

			// 1행, 2행의 3열을 0으로 만들어준다.
			record = Raws[0][2] / Raws[2][2];
			L.Raws[0] -= Raws[2] * record;
			U.Raws[0][2] -= record;
			record = Raws[1][2] / Raws[2][2];
			L.Raws[1] -= Raws[2] * record;
			U.Raws[1][2] -= record;

			// 1행 2열을 0으로 만들어준다
			record = Raws[0][1] / Raws[1][1];
			L.Raws[0] -= Raws[1] * record;
			U.Raws[0][1] -= record;

			// 대각 성분을 모두 1로 만들어 준다.
			float inverseOp = 1 / L.Raws[0][0];
			L.Raws[0] *= inverseOp;
			U.Raws[0] *= inverseOp;
			inverseOp = 1 / L.Raws[1][1];
			L.Raws[1] *= inverseOp;
			U.Raws[1] *= inverseOp;
			inverseOp = 1 / L.Raws[2][2];
			L.Raws[2] *= inverseOp;
			U.Raws[2] *= inverseOp;

			return U;
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



	FORCEINLINE Matrix2x2 Matrix3x3::ToMatrix2x2() const
	{
		return Matrix2x2(Raws[0].ToVector2(), Raws[1].ToVector2());
	}
