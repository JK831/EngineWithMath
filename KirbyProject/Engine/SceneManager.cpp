#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include "DataManager.h"
#include "TestCameraScript.h"
#include "Resources.h"

#include "tinyxml2.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene == nullptr)
		return;

	const vector<shared_ptr<GameObject>>& gameObjects = _activeScene->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->Render();
	}
}

void SceneManager::LoadScene(wstring InSceneName)
{
	if (InSceneName._Equal(L"TestScene"))
		_activeScene = LoadTestScene();
	else
	{
		shared_ptr<Scene> scene = make_shared<Scene>();

		unordered_map<wstring, vector<string>> componentMap;

		string sceneName;
		sceneName.assign(InSceneName.begin(), InSceneName.end());
		sceneName.append(".xml");

		tinyxml2::XMLDocument sceneFile;
		sceneFile.LoadFile(sceneName.c_str());


		// Load scene information 
		tinyxml2::XMLElement* root = sceneFile.RootElement();
		tinyxml2::XMLElement* object = root->FirstChildElement();
		for (tinyxml2::XMLElement* nextObj = object; nextObj != NULL; nextObj->NextSiblingElement())
		{
			uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));

			switch (static_cast<OBJECT_TYPE>(objType))
			{
			case OBJECT_TYPE::GAMEOBJECT:
			{
				shared_ptr<GameObject> gameObj = make_shared<GameObject>();
				static_pointer_cast<GameObject>(obj);

				string name = nextObj->Attribute("m_Name");
				wstring wName = wName.assign(name.begin(), name.end());
				gameObj->SetName(wName);
				
				for (tinyxml2::XMLElement* nextComponent = nextObj->FirstChildElement("m_Component"); nextComponent != NULL; nextComponent->NextSiblingElement())
				{
					gameObj->AddComponent(GET_SINGLE(DataManager)->GetComponentByID(nextComponent->GetText()));
				}
				scene->AddGameObject(gameObj);
			}
			case OBJECT_TYPE::MATERIAL:
			{
				shared_ptr<Material> mat = make_shared<Material>();


			}
			case OBJECT_TYPE::MESH:
			{
				obj = make_shared<Mesh>();
			}
			case OBJECT_TYPE::SHADER:
			{
				obj = make_shared<Shader>();
			}
			case  OBJECT_TYPE::COMPONENT:
			{
				obj = make_shared<Component>();

			}
			case OBJECT_TYPE::SPRITE:
			{
				
			}
			case OBJECT_TYPE::TEXTURE:
			{
				obj = make_shared<Texture>();
			}
			}
			

			

			scene->AddGameObject(go);
		}
	}

	for (tinyxml2::XMLElement* nextComponent = )

	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->AddComponent(make_shared<Transform>());
	camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45µµ
	camera->AddComponent(make_shared<TestCameraScript>());
	camera->GetTransform()->SetLocalPosition(Vector2(0.f, 0.f));
	scene->AddGameObject(camera);
#pragma endregion

#pragma region Rectangle
	{
		shared_ptr<GameObject> rectangle = make_shared<GameObject>();
		rectangle->AddComponent(make_shared<Transform>());
		rectangle->GetTransform()->SetLocalScale(Vector2(300.f, 300.f));
		rectangle->GetTransform()->SetLocalPosition(Vector2(0.f, 0.f));
		rectangle->GetTransform()->SetLocalRotation(45.f);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> rectangleMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(rectangleMesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"Default 2D Shader");
			texture->Init(L"..\\Resources\\Texture\\Kirby.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		rectangle->AddComponent(meshRenderer);
		scene->AddGameObject(rectangle);
	}
#pragma endregion

	return scene;
}
