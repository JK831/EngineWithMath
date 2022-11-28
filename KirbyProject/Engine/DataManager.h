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

private:
	map<string, Component> _ComponentMap;
};