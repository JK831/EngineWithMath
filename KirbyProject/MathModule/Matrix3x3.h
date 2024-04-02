#pragma once


	class Matrix3x3
	{
	public:
		// ������ 
		FORCEINLINE constexpr Matrix3x3() = default;
		FORCEINLINE explicit constexpr Matrix3x3(const Vector3& InRaw0, const Vector3& InRaw1, const Vector3& InRaw2) { Raws = { InRaw0, InRaw1, InRaw2 }; }

		// ������ 
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

		// ����Լ� 
		FORCEINLINE Matrix2x2 ToMatrix2x2() const;
		FORCEINLINE void SetIdentity();
		FORCEINLINE Matrix3x3 Transpose() const;
		FORCEINLINE Matrix3x3 Invert() const
		{
			/*-------------------------------------------------------------------
				���콺-�丣�� �ҰŹ��� ����Ͽ� ����� ������� ���Ѵ�.
				AX = I -> A�� �⺻ �� ������ ����Ͽ� I�� ����� �������� ���� ������
				I�� �״�� ������ �� X (A�� �����) ���� �� �ִ�.
			 --------------------------------------------------------------------*/

			 /*-------------------------------------------------------------------
				 LU ���ظ� ����Ͽ� ����� ��ﰢ, �ϻﰢ ����� ������ ������ְ�
				 determinant�� ���Ͽ� ��Ľ��� ���� �� �ִ�.
				 A = LU -> A = | a b c || g 0 0 |
							   | 0 d e || h i 0 |
							   | 0 0 f || j k l |
			  --------------------------------------------------------------------*/
			  // -> ��� ����� ��ﰢ ��ķ� ���� ����� L
			  //	  ���� ��� I�� ��ﰢ ��ķ� ����� �� �� ���� ������ ���� ������ �� = U.
			  // �߰�: pLU ���ظ� ������� �ʾƵ� �Ǵ� ���� => � GameObject�� World matrix (transform) = SRT
			  // S�� Scale ��ķμ� 3���� �������� ���ǰ� �ִ� ������Ʈ�� ��� �밢������ 0�� �� �� ����.

			Matrix3x3 L(Raws[0], Raws[1], Raws[2]);
			Matrix3x3 U; // AX = I ���� X�� �� ���, (�ʱ� �� = I)
			U.SetIdentity();

			// 1���� ���� �⺻ �� ���� -> Raw[0][0] (�״��) Raw[1][0] = 0 Raw[2][0] = 0 �� ���°� �ȴ�.
			float record = Raws[1][0] / Raws[0][0];
			L.Raws[1] -= Raws[0] * record;
			U.Raws[1][0] -= record; // ��ϵ� ���� �����ش�.

			record = Raws[2][0] / Raws[0][0];
			L.Raws[2] -= Raws[0] * record;
			U.Raws[2][0] -= record;

			// 2���� ���� �⺻ �� ���� -> Raw[0][1] Raw[1][1] Raw[2][1] = 0�� ���°� �ȴ�.
			record = Raws[2][1] / Raws[1][1];
			L.Raws[2] -= Raws[1] * record;
			U.Raws[2][1] -= record;

			// 1��, 2���� 3���� 0���� ������ش�.
			record = Raws[0][2] / Raws[2][2];
			L.Raws[0] -= Raws[2] * record;
			U.Raws[0][2] -= record;
			record = Raws[1][2] / Raws[2][2];
			L.Raws[1] -= Raws[2] * record;
			U.Raws[1][2] -= record;

			// 1�� 2���� 0���� ������ش�
			record = Raws[0][1] / Raws[1][1];
			L.Raws[0] -= Raws[1] * record;
			U.Raws[0][1] -= record;

			// �밢 ������ ��� 1�� ����� �ش�.
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

		// ����������� 
		static const Matrix3x3 Identity;
		static constexpr BYTE Rank = 3;

		// ������� 
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
