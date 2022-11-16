#include "pch.h"

WindowsGDI::~WindowsGDI()
{
}

bool WindowsGDI::InitializeGDI(const WindowInfo& windowInfo)
{
	ReleaseGDI();

	_window = windowInfo;

	if (_window.hwnd == 0)
	{
		_window.hwnd = ::GetActiveWindow();
		if (_window.hwnd == 0)
		{
			return false;
		}
	}

	if (_GDIInitialized)
	{
		DeleteObject(_DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(_window.hwnd, _ScreenDC);
		ReleaseDC(_window.hwnd, _MemoryDC);
	}

	_ScreenDC = GetDC(_window.hwnd);
	if (_ScreenDC == NULL)
	{
		return false;
	}

	_MemoryDC = CreateCompatibleDC(_ScreenDC);
	if (_MemoryDC == NULL)
	{
		return false;
	}


	// Color & Bitmap Setting
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = _window.width;
	bmi.bmiHeader.biHeight = -_window.height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DIBitmap = CreateDIBSection(_MemoryDC, &bmi, DIB_RGB_COLORS, (void**)&_ScreenBuffer, NULL, 0);
	if (DIBitmap == NULL)
	{
		return false;
	}

	_DefaultBitmap = (HBITMAP)SelectObject(_MemoryDC, DIBitmap);
	if (_DefaultBitmap == NULL)
	{
		return false;
	}

	// Create Depth Buffer
	CreateDepthBuffer();

	_GDIInitialized = true;
	return true;
}

void WindowsGDI::ReleaseGDI()
{
	if (_GDIInitialized)
	{
		DeleteObject(_DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(_window.hwnd, _ScreenDC);
		ReleaseDC(_window.hwnd, _MemoryDC);
	}

	if (_DepthBuffer != nullptr)
	{
		delete[] _DepthBuffer;
		_DepthBuffer = nullptr;
	}

	_GDIInitialized = false;
}


void WindowsGDI::FillBuffer(Color32 InColor)
{
	if (!_GDIInitialized || (_ScreenBuffer == NULL))
	{
		return;
	}

	Color32* dest = _ScreenBuffer;
	UINT32 totalCount = _window.width * _window.height;
	CopyBuffer<Color32>(_ScreenBuffer, &InColor, totalCount);
	return;
}

template <class T>
T* WindowsGDI::CopyBuffer(T* InDst, T* InSrc, int InCount)
{
	if (InCount == 0)
	{
		return NULL;
	}

	if (InCount == 1)
	{
		memcpy(InDst, InSrc, sizeof(T));
	}
	else
	{
		int half = Math::FloorToInt(InCount * 0.5f);
		CopyBuffer<T>(InDst, InSrc, half);
		memcpy(InDst + half, InDst, half * sizeof(T));

		if (InCount % 2)
		{
			memcpy(InDst + (InCount - 1), InSrc, sizeof(T));
		}
	}

	return InDst;
}

Color32* WindowsGDI::GetScreenBuffer() const
{
	return _ScreenBuffer;
}

void WindowsGDI::DrawStatisticTexts()
{
	if (_StatisticTexts.size() == 0)
	{
		return;
	}

	HFONT hFont, hOldFont;
	hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
	if (hOldFont = (HFONT)SelectObject(_MemoryDC, hFont))
	{
		static const int leftPosition = 600;
		static const int topPosition = 10;
		static const int rowHeight = 20;
		int currentPosition = topPosition;
		for (std::vector<std::string>::const_iterator it = _StatisticTexts.begin(); it != _StatisticTexts.end(); ++it)
		{
			TextOut(_MemoryDC, leftPosition, currentPosition, (*it).c_str(), (int)((*it).length()));
			currentPosition += rowHeight;
		}

		SelectObject(_MemoryDC, hOldFont);
	}
}

void WindowsGDI::SwapBuffer()
{
	if (!_GDIInitialized)
	{
		return;
	}

	DrawStatisticTexts();
	BitBlt(_ScreenDC, 0, 0, _window.width, _window.height, _MemoryDC, 0, 0, SRCCOPY);

	_StatisticTexts.clear();
}

void WindowsGDI::CreateDepthBuffer()
{
	_DepthBuffer = new float[_window.width * _window.height];
}

void WindowsGDI::ClearDepthBuffer()
{
	if (_DepthBuffer != nullptr)
	{
		float* dest = _DepthBuffer;
		static float defValue = INFINITY;
		UINT32 totalCount = _window.width * _window.height;
		CopyBuffer<float>(_DepthBuffer, &defValue, totalCount);
	}
}
