#pragma once
#include "Component.h"



class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	virtual void RegisterToDataManager();

public:
	template<typename T>
	shared_ptr<MonoBehaviour> CreateInstance() { return make_shared(T); }

private:
	string guid;
	
};
