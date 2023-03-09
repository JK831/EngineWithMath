#pragma once


	class Matrix4x4
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

		// ���Ϳ��� ������ ���� operator overloading
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


		// ����Լ� 
		FORCEINLINE Matrix3x3 ToMatrix3x3() const;
		FORCEINLINE void SetIdentity();
		FORCEINLINE constexpr Matrix4x4 Transpose() const;
		FORCEINLINE Matrix4x4 Invert() const
		{


			Matrix4x4 L(Raws[0], Raws[1], Raws[2], Raws[3]);
			Matrix4x4 U; // AX = I ���� X�� �� ���, (�ʱ� �� = I)
			U.SetIdentity();

			// ��ﰢ��� ������ش�
			/*	| a b c d |
				| 0 e f g |
				| 0 0 h i |
				| 0 0 0 j | */

			// 1���� ���� �⺻ �� ���� -> Raw[0][0] (�״��) & Raw[1][0], Raw[2][0], Raw[3][0] = 0 �� ���°� �ȴ�.
			float record = Raws[1][0] / Raws[0][0];
			L.Raws[1] += Raws[0] * (-1) * record;
			U.Raws[1][0] += record; // ��ϵ� ���� �����ش�.

			record = Raws[2][0] / Raws[0][0];
			L.Raws[2] += Raws[0] * (-1) * record;
			U.Raws[2][0] += record;

			record = Raws[3][0] / Raws[0][0];
			L.Raws[3] += Raws[0] * (-1) * record;
			U.Raws[3][0] += record;

			// 2���� ���� �⺻ �� ���� -> Raw[0][1], Raw[1][1] (�״��) & Raw[2][1], Raw[3][1] = 0�� ���°� �ȴ�.
			record = Raws[2][1] / Raws[1][1];
			L.Raws[2] += Raws[1] * (-1) * record;
			U.Raws[2][1] += record;

			record = Raws[3][1] / Raws[1][1];
			L.Raws[3] += Raws[1] * (-1) * record;
			U.Raws[3][1] += record;

			// 3���� ���� �⺻ �� ���� ->Raw[0][2], Raw[1][2], Raw[2][2] (�״��) & Raw[3][2] = 0�� ���°� �ȴ�.
			record = Raws[3][2] / Raws[2][2];
			L.Raws[3] += Raws[2] * (-1) * record;
			U.Raws[3][2] += record;

			// 1��, 2��, 3���� 4���� 0���� ������ش�.
			record = Raws[0][3] / Raws[3][3];
			L.Raws[0] += Raws[3] * (-1) * record;
			U.Raws[0][2] += record;

			record = Raws[1][3] / Raws[3][3];
			L.Raws[1] += Raws[3] * (-1) * record;
			U.Raws[1][3] += record;

			record = Raws[2][3] / Raws[3][3];
			L.Raws[2] += Raws[3] * (-1) * record;
			U.Raws[2][3] += record;

			// 1��, 2���� 3���� 0���� ������ش�
			record = Raws[0][2] / Raws[2][2];
			L.Raws[0] += Raws[2] * (-1) * record;
			U.Raws[0][2] += record;

			record = Raws[1][2] / Raws[2][2];
			L.Raws[1] += Raws[2] * (-1) * record;
			U.Raws[1][2] += record;

			// 1�� 2���� 0���� ������ش�
			record = Raws[0][1] / Raws[1][1];
			L.Raws[0] += Raws[1] * (-1) * record;
			U.Raws[0][1] += record;

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
			inverseOp = 1 / L.Raws[3][3];
			L.Raws[3] *= inverseOp;
			U.Raws[3] *= inverseOp;

			return U;
		}

		FORCEINLINE Vector3 Translation() const noexcept;
		FORCEINLINE void Translation(Vector3& InVector) { Raws[3].X = InVector.X; Raws[3].Y = InVector.Y; Raws[3].Z = InVector.Z; }
		FORCEINLINE Vector3 Up() const noexcept { return Vector3(Raws[1].X, Raws[1].Y, Raws[1].Z); };
		FORCEINLINE void Up(Vector3& InVector) { Raws[1].X = InVector.X; Raws[1].Y = InVector.Y; }
		FORCEINLINE Vector3 Right() const noexcept { return Vector3(Raws[0].X, Raws[0].Y, Raws[0].Z); };
		FORCEINLINE void Right(Vector3& InVector) {
			Raws[0].X = InVector.X; Raws[0].Y = InVector.Y; Raws[0].Z = InVector.Z;
		};
		FORCEINLINE Vector3 Forward() const noexcept { return Vector3(Raws[2].X, Raws[2].Y, Raws[2].Z); }
		FORCEINLINE void Forward(const Vector3& InVector) noexcept { Raws[2].X = InVector.X; Raws[2].Y = InVector.Y; Raws[2].Z = InVector.Z; }
		FORCEINLINE Vector3 Backward() const noexcept { return Vector3(-Raws[2].X, -Raws[2].Y, -Raws[2].Z); }
		FORCEINLINE void Backward(const Vector3& InVector) noexcept { Raws[2].X = -InVector.X; Raws[2].Y = -InVector.Y; Raws[2].Z = -InVector.Z; }
		

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

