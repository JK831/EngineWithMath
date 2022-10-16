#pragma once

typedef unsigned char BYTE;
typedef unsigned int UINT32;
typedef signed long long INT64;

#define SMALL_NUMBER	(1.e-8f)

enum class BoundCheckResult : UINT32
{
	OUTSIDE = 0,
	INTERSECT,
	INSIDE,
};