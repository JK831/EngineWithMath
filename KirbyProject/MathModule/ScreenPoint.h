#pragma once


	struct ScreenPoint
	{
	public:
		FORCEINLINE constexpr ScreenPoint() = default;
		FORCEINLINE explicit constexpr ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
		FORCEINLINE explicit constexpr ScreenPoint(float InX, float InY) : X(Math::FloorToInt(InX)), Y(Math::FloorToInt(InY)) { }
		FORCEINLINE explicit constexpr ScreenPoint(const Vector2& InPos) : ScreenPoint(InPos.X, InPos.Y) {}

		constexpr ScreenPoint GetHalf() const { return ScreenPoint(Math::FloorToInt(0.5f * X), Math::FloorToInt(0.5f * Y)); }
		constexpr float AspectRatio() const { return (float)X / (float)Y; } // Y축 기준으로 측정
		FORCEINLINE constexpr bool HasZero() const { return (X == 0 || Y == 0); }

		FORCEINLINE static constexpr ScreenPoint ToScreenCoordinate(const ScreenPoint& InScreenSize, const Vector2& InPos)
		{
			return ScreenPoint(InPos.X + InScreenSize.X * 0.5f, -InPos.Y + InScreenSize.Y * 0.5f);
		}

		FORCEINLINE constexpr Vector2 ToCartesianCoordinate(const ScreenPoint& InScreenSize)
		{
			// 데카르트 좌표계는 화면 중심을 기준으로 양,음 Y값을 모두 쓰지만 스크린 좌표계는 양의 Y값만 쓴다.
			// 따라서 스크린 좌표계의 중간값을 더해주어 데카르트 좌표계에서 양,음 Y 값을 제대로 출력할 수 있도록 한다.
			return Vector2(X - InScreenSize.X * 0.5f + 0.5f, -(Y + 0.5f) + InScreenSize.Y * 0.5f);
		}

		FORCEINLINE constexpr ScreenPoint operator-(const ScreenPoint& InPoint) const;
		FORCEINLINE constexpr ScreenPoint operator+(const ScreenPoint& InPoint) const;

		int32 X = 0;
		int32 Y = 0;
	};

	FORCEINLINE constexpr ScreenPoint ScreenPoint::operator-(const ScreenPoint& InPoint) const
	{
		return ScreenPoint(X - InPoint.X, Y - InPoint.Y);
	}

	FORCEINLINE constexpr ScreenPoint ScreenPoint::operator+(const ScreenPoint& InPoint) const
	{
		return ScreenPoint(X + InPoint.X, Y + InPoint.Y);
	}

