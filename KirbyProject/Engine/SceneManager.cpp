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

#include <fstream>

#include "json/json.h"
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

void SceneManager::LoadScene(wstring scenePath)
{
#pragma region For Simple Test
	if (scenePath == L"Test")
	{
		_activeScene = LoadTestScene();

		_activeScene->Awake();
		_activeScene->Start();

		return;
	}
#pragma endregion
	
#pragma region SceneLoading
	// 모두 씬 파일 내 FileID를 통해 각자를 식별
	// 엔진 내에서 정의된 데이터가 아닌 데이터(texture, mesh 등)은 추후 GUID를 통해 실제 데이터를 불러오도록 한다.
	shared_ptr<Scene> scene = make_shared<Scene>();

	unordered_map<string, vector<string>> goComponentMap;
	unordered_map<string, shared_ptr<Component>> componentMap;

	unordered_map<string, pair<string, string>> mRContentMap;
	unordered_map<string, shared_ptr<Mesh>> meshMap;
	unordered_map<string, shared_ptr<Material>> matMap;

	

	std::ifstream sceneFile(scenePath);
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;

	Json::Value sceneSetting;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, sceneFile, &sceneSetting, &errs);

	assert(ok);

	// scene 내 Object parsing

	uint32 objectCount = sceneSetting.size();
	vector<string> goId(objectCount);
	vector<string> mRId(objectCount / 2);

	for (auto it = sceneSetting.begin(); it != sceneSetting.end(); it++)
	{
		Json::Value object = *it;
		string fileId = object["FileID"].asString();
		uint8 objectType = object["Object_Type"].asUInt();

		switch (static_cast<OBJECT_TYPE>(objectType))
		{
		case OBJECT_TYPE::GAMEOBJECT:
		{
			goId.push_back(fileId);
			for (auto iit = object["m_Component"].begin(); iit != object["m_component"].end(); iit++)
			{
				string componentID = iit->asString();
				wstring wComponentID(componentID.begin(), componentID.end());
				goComponentMap[fileId].push_back(componentID);
			}
			break;
		}
		case OBJECT_TYPE::MATERIAL:
		{
			// TODO
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"Default 2D Shader");
			texture->Init(L"..\\Resources\\Texture\\Kirby.png"); // guid를 통해 얻은 path로 
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);

			matMap[fileId] = material;

			break;
		}
		case OBJECT_TYPE::MESH:
		{
			// TODO
			shared_ptr<Mesh> mesh = make_shared<Mesh>();
			mesh = GET_SINGLE(Resources)->LoadCubeMesh(); // 추후 메쉬 파일 직접 읽어오도록 수정 필요
			meshMap[fileId] = mesh;
			break;
		}
		case OBJECT_TYPE::COMPONENT:
		{
			// resource manager에서 id로 데이터 가져온 후
			// Component type 별로 component 설정.

			shared_ptr<Component> com;

			uint8 componentType = object["Component_Type"].asUInt();

			switch (static_cast<COMPONENT_TYPE>(componentType))
			{
			case COMPONENT_TYPE::TRANSFORM:
			{
				shared_ptr<Transform> transform = make_shared<Transform>();
				Json::Value position = object["Position"];
				float px = position[0].asFloat();
				float py = position[1].asFloat();
				float pz = position[2].asFloat();

				Json::Value rotation = object["Rotation"];
				float rx = rotation[0].asFloat();
				float ry = rotation[1].asFloat();
				float rz = rotation[2].asFloat();

				transform->SetLocalPosition(Vector3(px, py, pz));
				transform->SetLocalRotation(rz);

				com = transform;
				break;
			}
			case COMPONENT_TYPE::MESH_RENDERER:
			{
				shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
				com = meshRenderer;
				mRId.push_back(fileId);
				mRContentMap[fileId].first = object["m_Mesh"].asString();
				mRContentMap[fileId].second = object["m_Materials"].asString();
				break;
			}

			case COMPONENT_TYPE::MONO_BEHAVIOUR:
			{
				string guid = object["GUID"].asString();
				wstring path = GET_SINGLE(Resources)->GetPathByGuid(guid);
				wstring scriptName(path.begin() + path.find_last_of(L"\\"), path.end());
				shared_ptr<MonoBehaviour> script = MonoBehaviour::GetScript(scriptName);
				com = script;
				break;
			}
			}
			componentMap[fileId] = com;
			break;
		}

		}

	}

#pragma region XML(NotUsed)
	//tinyxml2::XMLDocument sceneFile;
	//sceneFile.LoadFile(sceneName.c_str());


	//// Load scene information 
	//tinyxml2::XMLElement* root = sceneFile.RootElement();
	//tinyxml2::XMLElement* object = root->FirstChildElement();
	//for (tinyxml2::XMLElement* nextObj = object; nextObj != NULL; nextObj->NextSiblingElement())
	//{	
	//	string objID = nextObj->Attribute("FildID");
	//	wstring wObjID = wObjID.assign(objID.begin(), objID.end());
	//	uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));
	//	
	//	switch (static_cast<OBJECT_TYPE>(objType))
	//	{
	//	case OBJECT_TYPE::GAMEOBJECT:
	//	{
	//		/*if (goComponentMap.find(wObjID) == goComponentMap.end())
	//		{
	//			vector<wstring> componentVector;
	//			goComponentMap[wObjID] = componentVector;
	//		}*/
	//			
	//		shared_ptr<GameObject> go = make_shared<GameObject>();

	//		for (tinyxml2::XMLElement* nextComponent = nextObj->FirstChildElement("m_Component"); nextComponent != NULL; nextComponent->NextSiblingElement("m_Component"))
	//		{
	//			// map에 gameobject id, component id 등록 (component가 이미 있을 시 찾아서 넣는다)
	//			string componentID = nextComponent->GetText();
	//			wstring wComponentID = wComponentID.assign(componentID.begin(), componentID.end());
	//			
	//			bool flag = true;
	//			if (goComponentMap[wObjID].find(wComponentID) != goComponentMap[wObjID].end())
	//			{
	//				go->AddComponent((GET_SINGLE(Resources)->LoadRegisteredAsset<Component>(wComponentID)));
	//				flag = false;
	//				break;
	//			}
	//			if (flag)
	//			{
	//				// 나중에 component가 자신의 정보를 등록하는 시점에 componentGOMap에 저장되어 있는 go에 AddComponent 수행
	//				componentGOMap[wComponentID] = go;
	//			}
	//		}
	//		scene->AddGameObject(go);
	//	}
	//	case OBJECT_TYPE::MATERIAL:
	//	{
	//		// TODO
	//	}
	//	case OBJECT_TYPE::MESH:
	//	{
	//		// TODO
	//	}
	//	case OBJECT_TYPE::SHADER:
	//	{
	//		// TODO
	//	}
	//	case OBJECT_TYPE::COMPONENT:
	//	{
	//		// resource manager에서 id로 데이터 가져온 후
	//		// Component type 별로 component 설정.
	//		shared_ptr<Component> com;
	//		
	//		uint8 componentType = std::stoi(nextObj->FirstChildElement("m_ComponentType")->GetText());

	//		switch (static_cast<COMPONENT_TYPE>(componentType))
	//		{
	//		case COMPONENT_TYPE::TRANSFORM:
	//		{
	//			tinyxml2::XMLElement* positionElement = nextObj->FirstChildElement("Position");
	//			tinyxml2::XMLElement* rotationElement = nextObj->FirstChildElement("Rotation");
	//			float px = std::stod(positionElement->FirstChildElement("X")->GetText());
	//			float py = std::stod(positionElement->FirstChildElement("Y")->GetText());
	//			float pz = std::stod(positionElement->FirstChildElement("Z")->GetText());
	//			float rx = std::stod(rotationElement->FirstChildElement("X")->GetText());
	//			float ry = std::stod(rotationElement->FirstChildElement("Y")->GetText());
	//			float rz = std::stod(rotationElement->FirstChildElement("Z")->GetText());
	//			
	//			shared_ptr<Transform> transform;
	//			transform->SetLocalPosition(Vector2(px, py));
	//			transform->SetLocalRotation(rz);

	//			transformMap[objID] = transform;

	//			// 부모, 자식 transform이 있다면 이미 등록되어 있는지 체크 후 등록되어 있다면 자신에게 SetParent를, 자식에게 SetParent(자신)을 해준다.
	//			tinyxml2::XMLElement* parentTransform = nextObj->FirstChildElement("m_Parent");
	//			if (parentTransform != NULL)
	//			{
	//				if (transformMap.find(parentTransform->Attribute("FileID")) != transformMap.end())
	//					transform->SetParent(transformMap[parentTransform->Attribute("FileID")]);
	//			}

	//			for (tinyxml2::XMLElement* childTransform = nextObj->FirstChildElement("m_Childeren"); childTransform != NULL; childTransform = childTransform->NextSiblingElement())
	//			{
	//				if (transformMap.find(childTransform->Attribute("FileID")) != transformMap.end())
	//					transformMap[childTransform->Attribute("FileID")]->SetParent(transform);
	//			}

	//			com = transform;
	//			break;
	//		}
	//		case COMPONENT_TYPE::MONO_BEHAVIOUR:
	//		{
	//			com = GET_SINGLE(Resources)->LoadRegisteredAsset<MonoBehaviour>(wObjID);
	//			break;
	//		}
	//		}

	//		if (componentGOMap.find(wObjID) != componentGOMap.end())
	//		{
	//			componentGOMap[wObjID]->AddComponent(com);
	//		}
	//		else
	//		{
	//			string goID = nextObj->FirstChildElement("m_GameObject")->GetText();
	//			wstring wGameObjectID(goID.begin(), goID.end());
	//			goComponentMap[wGameObjectID][wObjID] = com;
	//		}
	//	}

	//	}

	//	// objType에 따라 Resources::Load 호출
	//	
	//}
#pragma endregion

	// GameObject들의 fileId 별로 갖고있는 컴포넌트의 id로 componentMap에서 component 가져와서 add
	for (string fileId : goId)
	{
		shared_ptr<GameObject> go;
		
		for (auto compo : goComponentMap[fileId])
			go->AddComponent(componentMap[compo]);

		scene->AddGameObject(go);
	}

	for (string fileId : mRId)
	{
		auto meshRenderer = static_pointer_cast<MeshRenderer>(componentMap[fileId]);
		meshRenderer->SetMesh(meshMap[mRContentMap[fileId].first]);
		meshRenderer->SetMaterial(matMap[mRContentMap[fileId].second]);
	}

	_activeScene = scene;
#pragma endregion


	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->AddComponent(make_shared<Transform>());
	camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
	camera->AddComponent(make_shared<TestCameraScript>());
	camera->GetTransform()->SetLocalPosition(Vector3::Zero);
	scene->AddGameObject(camera);
#pragma endregion

#pragma region Cube
	{
		shared_ptr<GameObject> cube = make_shared<GameObject>();
		cube->AddComponent(make_shared<Transform>());
		cube->GetTransform()->SetLocalScale(Vector3(300.f, 300.f, 300.f));
		cube->GetTransform()->SetLocalPosition(Vector3::Zero);
		cube->GetTransform()->SetLocalRotation(45.f);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(cubeMesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"Default 3D Shader");
			texture->Init(L"..\\Resources\\Texture\\Kirby.png"); // guid를 통해 얻은 path로 
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		cube->AddComponent(meshRenderer);
		scene->AddGameObject(cube);
	}
#pragma endregion

//#pragma region cube2
//	{
//		shared_ptr<GameObject> cube = make_shared<GameObject>();
//		cube->AddComponent(make_shared<Transform>());
//		cube->GetTransform()->SetLocalScale(Vector2(300.f, 300.f));
//		cube->GetTransform()->SetLocalPosition(Vector2(100.f, 100.f));
//		cube->GetTransform()->SetLocalRotation(0.f);
//
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadcubeMesh();
//			meshRenderer->SetMesh(cubeMesh);
//		}
//		{
//			shared_ptr<Shader> shader = make_shared<Shader>();
//			shared_ptr<Texture> texture = make_shared<Texture>();
//			shader->Init(L"Default 2D Shader");
//			texture->Init(L"..\\Resources\\Texture\\Kirby.png");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		cube->AddComponent(meshRenderer);
//		scene->AddGameObject(cube);
//	}
//#pragma endregion

	return scene;
}

shared_ptr<Scene> SceneManager::LoadMainScene()
{
	return nullptr;
}