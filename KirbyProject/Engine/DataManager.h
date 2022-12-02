#pragma once

class Object;
class Component;

class DataManager
{
	DECLARE_SINGLE(DataManager);

public:
	DataManager();
	~DataManager();


	void Register();
	
	shared_ptr<Component> GetComponentByID(string InFileID) { return make_shared(_ComponentMap.find(InFileID)); }

private:
	map<string, Object> _ObjectMap;
	map<string, Component> _ComponentMap;
};