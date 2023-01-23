#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	GET_SINGLE(SceneManager)->LoadScene(L"Test"); // �� ��� ��ü�� �����Ͽ� �ε��ϵ��� �Ѵ�
}

void Game::Update()
{
	GEngine->Update();
}