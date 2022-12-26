#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

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
#pragma region SceneLoading
		// ��� �� ���� �� FileID�� ���� ���ڸ� �ĺ�
		// ���� ������ ���ǵ� �����Ͱ� �ƴ� ������(texture, mesh ��)�� ���� GUID�� ���� ���� �����͸� �ҷ������� �Ѵ�.
		shared_ptr<Scene> scene = make_shared<Scene>();

		unordered_map<wstring, unordered_map<wstring, shared_ptr<Component>>> goComponentMap;
		unordered_map<wstring, shared_ptr<GameObject>> componentGOMap;
		unordered_map<string, shared_ptr<Transform>> transformMap;

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
			string objID = nextObj->Attribute("FildID");
			wstring wObjID = wObjID.assign(objID.begin(), objID.end());
			uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));
			
			switch (static_cast<OBJECT_TYPE>(objType))
			{
			case OBJECT_TYPE::GAMEOBJECT:
			{
				/*if (goComponentMap.find(wObjID) == goComponentMap.end())
				{
					vector<wstring> componentVector;
					goComponentMap[wObjID] = componentVector;
				}*/
					
				shared_ptr<GameObject> go = make_shared<GameObject>();

				for (tinyxml2::XMLElement* nextComponent = nextObj->FirstChildElement("m_Component"); nextComponent != NULL; nextComponent->NextSiblingElement("m_Component"))
				{
					// map�� gameobject id, component id ��� (component�� �̹� ���� �� ã�Ƽ� �ִ´�)
					string componentID = nextComponent->GetText();
					wstring wComponentID = wComponentID.assign(componentID.begin(), componentID.end());
					
					bool flag = true;
					if (goComponentMap[wObjID].find(wComponentID) != goComponentMap[wObjID].end())
					{
						go->AddComponent((GET_SINGLE(Resources)->LoadRegisteredAsset<Component>(wComponentID)));
						flag = false;
						break;
					}
					if (flag)
					{
						// ���߿� component�� �ڽ��� ������ ����ϴ� ������ componentGOMap�� ����Ǿ� �ִ� go�� AddComponent ����
						componentGOMap[wComponentID] = go;
					}
				}
				scene->AddGameObject(go);
			}
			case OBJECT_TYPE::MATERIAL:
			{
				// TODO
			}
			case OBJECT_TYPE::MESH:
			{
				// TODO
			}
			case OBJECT_TYPE::SHADER:
			{
				// TODO
			}
			case OBJECT_TYPE::COMPONENT:
			{
				// resource manager���� id�� ������ ������ ��
				// Component type ���� component ����.
				shared_ptr<Component> com;
				
				uint8 componentType = std::stoi(nextObj->FirstChildElement("m_ComponentType")->GetText());

				switch (static_cast<COMPONENT_TYPE>(componentType))
				{
				case COMPONENT_TYPE::TRANSFORM:
				{
					tinyxml2::XMLElement* positionElement = nextObj->FirstChildElement("Position");
					tinyxml2::XMLElement* rotationElement = nextObj->FirstChildElement("Rotation");
					float px = std::stod(positionElement->FirstChildElement("X")->GetText());
					float py = std::stod(positionElement->FirstChildElement("Y")->GetText());
					float pz = std::stod(positionElement->FirstChildElement("Z")->GetText());
					float rx = std::stod(rotationElement->FirstChildElement("X")->GetText());
					float ry = std::stod(rotationElement->FirstChildElement("Y")->GetText());
					float rz = std::stod(rotationElement->FirstChildElement("Z")->GetText());
					
					shared_ptr<Transform> transform;
					transform->SetLocalPosition(Vector2(px, py));
					transform->SetLocalRotation(rz);

					transformMap[objID] = transform;

					// �θ�, �ڽ� transform�� �ִٸ� �̹� ��ϵǾ� �ִ��� üũ �� ��ϵǾ� �ִٸ� �ڽſ��� SetParent��, �ڽĿ��� SetParent(�ڽ�)�� ���ش�.
					tinyxml2::XMLElement* parentTransform = nextObj->FirstChildElement("m_Parent");
					if (parentTransform != NULL)
					{
						if (transformMap.find(parentTransform->Attribute("FileID")) != transformMap.end())
							transform->SetParent(transformMap[parentTransform->Attribute("FileID")]);
					}

					for (tinyxml2::XMLElement* childTransform = nextObj->FirstChildElement("m_Childeren"); childTransform != NULL; childTransform = childTransform->NextSiblingElement())
					{
						if (transformMap.find(childTransform->Attribute("FileID")) != transformMap.end())
							transformMap[childTransform->Attribute("FileID")]->SetParent(transform);
					}

					com = transform;
					break;
				}
				case COMPONENT_TYPE::MONO_BEHAVIOUR:
				{
					com = GET_SINGLE(Resources)->LoadRegisteredAsset<MonoBehaviour>(wObjID);
					break;
				}
				}

				if (componentGOMap.find(wObjID) != componentGOMap.end())
				{
					componentGOMap[wObjID]->AddComponent(com);
				}
				else
				{
					string goID = nextObj->FirstChildElement("m_GameObject")->GetText();
					wstring wGameObjectID = wGameObjectID.assign(goID.begin(), goID.end());
					goComponentMap[wGameObjectID][wObjID] = com;
				}
			}

			}

			// objType�� ���� Resources::Load ȣ��
			
		}
		_activeScene = scene;
#pragma endregion
	}


	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->AddComponent(make_shared<Transform>());
	camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
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

#pragma region Rectangle2
	{
		shared_ptr<GameObject> rectangle = make_shared<GameObject>();
		rectangle->AddComponent(make_shared<Transform>());
		rectangle->GetTransform()->SetLocalScale(Vector2(300.f, 300.f));
		rectangle->GetTransform()->SetLocalPosition(Vector2(100.f, 100.f));
		rectangle->GetTransform()->SetLocalRotation(0.f);

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

shared_ptr<Scene> SceneManager::LoadMainScene()
{
	return nullptr;
}