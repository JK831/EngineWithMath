#pragma once

#include "resource.h"
#include "pch.h"
#include "Engine.h"
#include "Resources.h"

#include "tinyxml2.h"

class SceneEditor
{
public:
	void Init(const WindowInfo& windowInfo); // 씬을 엔진의 Resources를 활용하여 수정할 수 있게 한다.
	void Update();

	void AddGameObject();
	void RemoveGameObject();

	void AddComponentToGO(wstring& gameObjectID, wstring& componentID);
	void RemoveComponentFromGO(wstring& gameObjectID, wstring& componentID);

	void ParseSceneFile(wstring& ScenePath);

};

void SceneEditor::Init(const WindowInfo& windowInfo)
{
	GEngine->Init(windowInfo);
}

void SceneEditor::Update()
{

}

void SceneEditor::ParseSceneFile(wstring& InScenePath)
{
	string scenePath;
	scenePath.assign(InScenePath.begin(), InScenePath.end());

	tinyxml2::XMLDocument sceneFile;
	sceneFile.LoadFile(scenePath.c_str());


	//// Load scene information 
	//tinyxml2::XMLElement* root = sceneFile.RootElement();
	//tinyxml2::XMLElement* object = root->FirstChildElement();
	//for (tinyxml2::XMLElement* nextObj = object; nextObj != NULL; nextObj->NextSiblingElement())
	//{
	//	string objID = nextObj->Attribute("FildID");
	//	wstring wObjID = wObjID.assign(objID.begin(), objID.end());
	//	uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));

	//	switch (static_cast<OBJECT_TYPE>(objType))
	//	{
	//	case OBJECT_TYPE::GAMEOBJECT:
	//	{
	//		/*if (obConcernedMap.find(wObjID) == obConcernedMap.end())
	//		{
	//			vector<wstring> componentVector;
	//			obConcernedMap[wObjID] = componentVector;
	//		}*/


	//		for (tinyxml2::XMLElement* nextComponent = nextObj->FirstChildElement("m_Component"); nextComponent != NULL; nextComponent->NextSiblingElement("m_Component"))
	//		{
	//			string componentID = nextComponent->GetText();
	//			wstring wComponentID = wComponentID.assign(componentID.begin(), componentID.end());

	//			goComponentsMap[wobjID].push_back(wComponentID);
	//			
	//		}

	//	}
	//	case OBJECT_TYPE::COMPONENT:
	//	{

	//		tinyxml2::XMLElement* parentGO = nextObj->FirstChildElement("m_Parent");
	//		string parentID = parentGO->GetText();
	//		wstring wComponentID = wComponentID.assign(parentID.begin(), parentID.end());
	//		uint8 componentType = std::stoi(nextObj->FirstChildElement("m_ComponentType")->GetText());

	//		componentInfoMap[wComponentID] = componentType;
	//	}
	//	

	//	}

	//	// objType에 따라 Resources::Load 호출

	//}
}