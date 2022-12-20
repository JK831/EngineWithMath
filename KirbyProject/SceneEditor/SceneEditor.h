#pragma once

#include "resource.h"
#include "pch.h"
#include "Engine.h"
#include "Resources.h"

class SceneEditor
{
public:
	void Init(const WindowInfo& windowInfo); // 씬을 엔진의 Resources를 활용하여 수정할 수 있게 한다.
	void Update();

	void AddGameObject();
	void RemoveGameObject();

	void AddComponentToGO(wstring& gameObjectID, wstring& componentID);
	void RemoveComponentFromGO(wstring& gameObjectID, wstring& componentID);

};

void SceneEditor::Init(const WindowInfo& windowInfo)
{
	GEngine->Init(windowInfo);
}

void SceneEditor::Update()
{

}