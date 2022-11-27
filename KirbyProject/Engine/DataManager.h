#pragma once

class MonoBehaviour;

class DataManager
{
	DECLARE_SINGLE(DataManager);

public:
	DataManager();
	~DataManager();

	template<typename T>
	void Register(T& InMono, string InGuid);

private:
	map<string, shared_ptr<MonoBehaviour>(*)()> _monoMap;
};