#pragma once


	struct ScreenPoint
	{
	public:
		FORCEINLINE constexpr ScreenPoint() = default;
		FORCEINLINE explicit constexpr ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
		FORCEINLINE explicit constexpr ScreenPoint(float InX, float InY) : X(Math::FloorToInt(InX)), Y(Math::FloorToInt(InY)) { }
		FORCEINLINE explicit constexpr ScreenPoint(const Vector2& InPos) : ScreenPoint(InPos.X, InPos.Y) {}

		constexpr ScreenPoint GetHalf() const { return ScreenPoint(Math::FloorToInt(0.5f * X), Math::FloorToInt(0.5f * Y)); }
		constexpr float AspectRatio() const { return (float)X / (float)Y; } // Y�� �������� ����
		FORCEINLINE constexpr bool HasZero() const { return (X == 0 || Y == 0); }

		FORCEINLINE static constexpr ScreenPoint ToScreenCoordinate(const WindowInfo& InWindowInfo, const Vector2& InPos)
		{
			return ScreenPoint(InPos.X + InWindowInfo.width * 0.5f, -InPos.Y + InWindowInfo.height * 0.5f);
		}

		FORCEINLINE constexpr Vector2 ToCartesianCoordinate(const WindowInfo& InWindowInfo)
		{
			// ��ī��Ʈ ��ǥ��� ȭ�� �߽��� �������� ��,�� Y���� ��� ������ ��ũ�� ��ǥ��� ���� Y���� ����.
			// ���� ��ũ�� ��ǥ���� �߰����� �����־� ��ī��Ʈ ��ǥ�迡�� ��,�� Y ���� ����� ����� �� �ֵ��� �Ѵ�.
			return Vector2(X - InWindowInfo.width * 0.5f + 0.5f, -(Y + 0.5f) + InWindowInfo.height * 0.5f);
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

