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
	virtual shared_ptr<MonoBehaviour> CreateInstance() = 0;
	
	static void RegisterScript(wstring scriptType, shared_ptr<MonoBehaviour> script) { _MonoTypeMap.insert(make_pair(scriptType, script)); }
	static shared_ptr<MonoBehaviour> GetScript(wstring& scriptName);

private:

	static std::unordered_map<wstring, shared_ptr<MonoBehaviour>> _MonoTypeMap;
};
