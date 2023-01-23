#include "pch.h"
#include "MonoBehaviour.h"
#include "Engine.h"

MonoBehaviour MonoBehaviour::_staticObject;
std::unordered_map<wstring, MonoBehaviour> MonoBehaviour::_MonoTypeMap;

MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{

}

MonoBehaviour::~MonoBehaviour()
{

}


shared_ptr<MonoBehaviour> MonoBehaviour::GetScript(wstring& scriptName)
{
	return _MonoTypeMap[scriptName].CreateInstance();
}