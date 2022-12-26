#pragma once

class Scene; // ���漱��

	class SceneManager
	{
		DECLARE_SINGLE(SceneManager);

	public:
		void Update();
		void Render();
		void LoadScene(wstring InSceneName);

	public:
		shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	private:
		shared_ptr<Scene> LoadTestScene();
		shared_ptr<Scene> LoadMainScene();

	private:
		shared_ptr<Scene> _activeScene;
	};