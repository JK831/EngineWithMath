#pragma once

#include "resource.h"
#include "pch.h"
#include "Engine.h"
#include "Resources.h"

class SceneEditor
{
public:
	void Init(const WindowInfo& windowInfo); // ���� ������ Resources�� Ȱ���Ͽ� ������ �� �ְ� �Ѵ�.
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