#include "pch.h"


const Matrix3x3 Matrix3x3::Identity(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f));

//Matrix3x3 Matrix3x3::Invert() const
//{
//	float d = Raws[0][0] * (Raws[1][1] * Raws[2][2] - Raws[2][1] * Raws[1][2]) - Raws[0][1] * (Raws[1][0] * Raws[2][2] - Raws[1][2] * Raws[2][0]) + Raws[0][2] * (Raws[1][0] * Raws[2][1] - Raws[1][1] * Raws[2][0]);
//	if (d != 0.)
//	{
//		Matrix3x3 invertedMatrix;
//		float id = 1 / d;
//		invertedMatrix.Raws[0][0] = (Raws[1][1] * Raws[2][2] - Raws[2][1] * Raws[1][2]) * id;
//		invertedMatrix.Raws[0][1] = (Raws[0][2] * Raws[2][1] - Raws[0][1] * Raws[2][2]) * id;
//		invertedMatrix.Raws[0][2] = (Raws[0][1] * Raws[1][2] - Raws[0][2] * Raws[1][1]) * id;
//		invertedMatrix.Raws[1][0] = (Raws[1][2] * Raws[2][0] - Raws[1][0] * Raws[2][2]) * id;
//		invertedMatrix.Raws[1][1] = (Raws[0][0] * Raws[2][2] - Raws[0][2] * Raws[2][0]) * id;
//		invertedMatrix.Raws[1][2] = (Raws[1][0] * Raws[0][2] - Raws[0][0] * Raws[1][2]) * id;
//		invertedMatrix.Raws[2][0] = (Raws[1][0] * Raws[2][1] - Raws[2][0] * Raws[1][1]) * id;
//		invertedMatrix.Raws[2][1] = (Raws[2][0] * Raws[0][1] - Raws[0][0] * Raws[2][1]) * id;
//		invertedMatrix.Raws[2][2] = (Raws[0][0] * Raws[1][1] - Raws[1][0] * Raws[0][1]) * id;
//
//		return invertedMatrix;
//	}
//}

Matrix3x3 Matrix3x3::CreateRotationZ(float InRadian)
{
	float sinVal = 0;
	float cosVal = 0;

	Math::GetSinCos(sinVal, cosVal, InRadian);

	return Matrix3x3(Vector3(cosVal, -sinVal, 0), Vector3(sinVal, cosVal, 0), Vector3::UnitZ);
}

Matrix3x3 Matrix3x3::CreateTranslation(Vector2 InPosition)
{
	return Matrix3x3(Vector3::UnitX, Vector3::UnitY, Vector3(InPosition));
}

std::vector<std::string> Matrix3x3::ToStrings() const
{
	std::vector<std::string> result;

	Matrix3x3 trMatrix = this->Transpose();
	for (BYTE i = 0; i < Rank; ++i)
	{
		char row[64];
		std::snprintf(row, sizeof(row), "| %.3f , %.3f , %.3f |", trMatrix.Raws[i].X, trMatrix.Raws[i].Y, trMatrix.Raws[i].Z);
		result.emplace_back(row);
	}
	return result;
}