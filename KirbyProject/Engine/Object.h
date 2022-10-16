#pragma once

namespace JK
{

	class Object
	{
	public:
		Object(OBJECT_TYPE type);
		virtual ~Object();

		OBJECT_TYPE GetType() { return _objectType; }

		void SetName(const wstring& name) { _name = name; }
		const wstring& GetName() { return _name; }

	protected:
		friend class Resources;
		virtual void Load(const wstring& path) {}
		virtual void Save(const wstring& path) {}

	protected:
		OBJECT_TYPE _objectType = OBJECT_TYPE::NONE;
		wstring _name;
	};

}