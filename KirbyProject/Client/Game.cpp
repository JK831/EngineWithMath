#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	GET_SINGLE(SceneManager)->LoadScene(L"TestScene"); // 나중엔 씬 경로 전체를 전달하여 로드하도록 한다
}

void Game::Update()
{
	GEngine->Update();
}