#include "pch.h"
#include "DataManager.h"
#include "MonoBehaviour.h"

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

template<typename T>
void DataManager::Register(T& InMono, string InGuid)
{
	_monoMap.insert(make_pair(InGuid, InMono.CreateInstance()));
}
