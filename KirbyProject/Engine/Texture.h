#pragma once
#include "Object.h"

	class Texture : JK::Object
	{
	public:
		Texture();
		virtual ~Texture();

		void Init(const wstring& path);
		void CreateTexture(const wstring& path);

	public:
		void Release();
		bool IsIntialized() const { return (_Buffer.size() > 0); }
		uint32 GetWidth() const { return _Width; }
		uint32 GetHeight() const { return _Height; }
		uint32 GetSize() const { return _Width * _Height; }
		LinearColor GetSample(Vector2 InUV) const;
		TextureFilterMode GetMagFilterMode() const { return _MagFilter; }
		TextureFilterMode GetMinFilterMode() const { return _MinFilter; }
		TextureAddressMode GetAddressMode() const { return _AddressMode; }
		bool IsAnisotropyEnabled() const { return _AnisotropyEnable; }
		float GetMaxAnisotropy() const { return _MaxAnisotropy; }

	private:
		void LoadFromFile(FILE* f);

	private:
		static constexpr BYTE _Channel = 4;
		::std::vector<LinearColor> _Buffer;
		uint32 _Width = 0;
		uint32 _Height = 0;

		TextureFilterMode _MagFilter = TextureFilterMode::Nearest;
		TextureFilterMode _MinFilter = TextureFilterMode::Nearest;
		TextureAddressMode _AddressMode = TextureAddressMode::Repeat;
		bool _AnisotropyEnable = false;
		float _MaxAnisotropy = 16.0f;
	};
