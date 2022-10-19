#pragma once

#include <cassert>

#include <math.h>

#include <intrin.h>
#include "Platform.h"

#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#include "MathUtil.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

#include "ScreenPoint.h"

#include "Color32.h"
#include "LinearColor.h"
#include "HSVColor.h"

#include "Rotator.h"

#include "Quaternion.h"

#include "Transform.h"

#include "Plane.h"

#include "Circle.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Box.h"

#include "Frustum.h"

class MathHeaders
{
};

