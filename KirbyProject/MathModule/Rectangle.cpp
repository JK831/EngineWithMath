#include "pch.h"

using namespace JK;

Rectangle::Rectangle(const std::vector<Vector2> InVertices)
{
	for (const auto& v : InVertices)
	{
		*this += v;
	}
}