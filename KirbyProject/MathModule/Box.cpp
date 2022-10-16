#include "pch.h"

using namespace JK;

Box::Box(const std::vector<Vector3> InVertices)
{
	for (const auto& v : InVertices)
	{
		*this += v;
	}
}
