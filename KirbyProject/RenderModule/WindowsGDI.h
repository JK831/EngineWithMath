#pragma once


	class WindowsGDI
	{
	public:
		WindowsGDI() = default;
		~WindowsGDI();

	public:
		bool InitializeGDI(const WindowInfo& windowInfo);
		void ReleaseGDI();

		void FillBuffer(Color32 InColor);

		FORCEINLINE LinearColor GetPixel(const ScreenPoint& InPos);
		FORCEINLINE void SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor);
		FORCEINLINE void SetPixelAlphaBlending(const ScreenPoint& InPos, const LinearColor& InColor);

		void CreateDepthBuffer();
		void ClearDepthBuffer();

		Color32* GetScreenBuffer() const;

		void DrawStatisticTexts();

		void SwapBuffer();

		WindowInfo& GetWindow() { return _window; }

	protected:
		FORCEINLINE bool IsInScreen(const ScreenPoint& InPos) const;
		int GetScreenBufferIndex(const ScreenPoint& InPos) const;

		template <class T>
		T* CopyBuffer(T* InDst, T* InSrc, int InCount);

	protected:
		bool _GDIInitialized = false;

		HWND _Handle = 0;
		HDC	_ScreenDC = 0, _MemoryDC = 0;
		HBITMAP _DefaultBitmap = 0, DIBitmap = 0;

		Color32* _ScreenBuffer = nullptr;
		float* _DepthBuffer = nullptr;

		ScreenPoint _ScreenSize;
		std::vector<std::string> _StatisticTexts;

		WindowInfo _window;

	};

	FORCEINLINE void WindowsGDI::SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor)
	{
		if (!IsInScreen(InPos))
		{
			return;
		}

		Color32* dest = _ScreenBuffer;
		*(dest + GetScreenBufferIndex(InPos)) = InColor.ToColor32();
		return;
	}

	FORCEINLINE void WindowsGDI::SetPixelAlphaBlending(const ScreenPoint& InPos, const LinearColor& InColor)
	{
		LinearColor bufferColor = GetPixel(InPos);
		if (!IsInScreen(InPos))
		{
			return;
		}

		Color32* dest = _ScreenBuffer;
		*(dest + GetScreenBufferIndex(InPos)) = (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32();
	}

	FORCEINLINE bool WindowsGDI::IsInScreen(const ScreenPoint& InPos) const
	{
		if ((InPos.X < 0 || InPos.X >= _window.width) || (InPos.Y < 0 || InPos.Y >= _window.height))
		{
			return false;
		}

		return true;
	}

	FORCEINLINE int WindowsGDI::GetScreenBufferIndex(const ScreenPoint& InPos) const
	{
		return InPos.Y * _window.width + InPos.X;
	}

	FORCEINLINE LinearColor WindowsGDI::GetPixel(const ScreenPoint& InPos)
	{
		if (!IsInScreen(InPos))
		{
			return LinearColor::Error;
		}

		Color32* dest = _ScreenBuffer;
		Color32 bufferColor = *(dest + GetScreenBufferIndex(InPos));
		return LinearColor(bufferColor);
	}
