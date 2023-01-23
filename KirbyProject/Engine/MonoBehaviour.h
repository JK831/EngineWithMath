#pragma once
#include "Component.h"



class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	//virtual void RegisterToDataManager();
public:
	virtual shared_ptr<MonoBehaviour> CreateInstance() { return make_shared<MonoBehaviour>(MonoBehaviour::_staticObject); };
	
	static void RegisterScript(wstring scriptType, MonoBehaviour script) { _MonoTypeMap.insert(make_pair(scriptType, script)); }
	static shared_ptr<MonoBehaviour> GetScript(wstring& scriptName);
	static MonoBehaviour _staticObject;
private:

	static std::unordered_map<wstring, MonoBehaviour> _MonoTypeMap;
};
