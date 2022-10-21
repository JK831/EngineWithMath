#pragma once

class Scene; // ���漱��

	class SceneManager
	{
		DECLARE_SINGLE(SceneManager);

	public:
		void Update();
		void Render();
		void LoadScene(wstring sceneName);

	public:
		shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	private:
		shared_ptr<Scene> LoadTestScene();

	private:
		shared_ptr<Scene> _activeScene;
	};