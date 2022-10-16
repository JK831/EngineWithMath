#include "pch.h"

using namespace JK;

const Matrix4x4 Matrix4x4::Identity(Vector4(1.f, 0.f, 0.f, 0.f), Vector4(0.f, 1.f, 0.f, 0.f), Vector4(0.f, 0.f, 1.f, 0.f), Vector4(0.f, 0.f, 0.f, 1.f));

std::vector<std::string> Matrix4x4::ToStrings() const
{
	std::vector<std::string> result;

	Matrix4x4 trMatrix = this->Transpose();
	for (BYTE i = 0; i < Rank; ++i)
	{
		char row[64];
		std::snprintf(row, sizeof(row), "| %.3f , %.3f , %.3f, %.3f |", trMatrix.Raws[i].X, trMatrix.Raws[i].Y, trMatrix.Raws[i].Z, trMatrix.Raws[i].W);
		result.emplace_back(row);
	}
	return result;
}