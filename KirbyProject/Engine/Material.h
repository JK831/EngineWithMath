#pragma once
#include "Object.h"
#include "Shader.h"
#include "Texture.h"

	enum
	{
		MATERIAL_TEXTURE_COUNT = 5,
	};

	class Material : public Object
	{
	public:
		Material();
		virtual ~Material();

		shared_ptr<Shader> GetShader() { return _shader; }

		void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
		void SetTexture(uint8 index, shared_ptr<Texture> texture) { _textures[index] = texture; }
		void SetIndexNum(uint16 InIndexNum) { _indexNum = InIndexNum; }
		const array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT>& GetTexture() { return _textures; }

		uint16 Update();

	private:
		shared_ptr<Shader> _shader;
		array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;
		uint16 _indexNum;
	};
