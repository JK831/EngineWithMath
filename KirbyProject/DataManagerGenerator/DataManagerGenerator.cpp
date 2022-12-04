// DataManagerGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "EnginePch.h"

#include <io.h>
#include "tinyxml2.h"

void SearchingDir(string path, string& InToInclude, string& InToWrite, char InToParse);
bool IsFileOrDir(_finddata_t fd);
void ParseAssetFiles(string InPath, string& InToInclude, string& InFileWrite);
void ParseSceneFiles(string InPath, string& InToInclude, string& InFileWrite);

int main()
{
	string assetPath = "..\\Resources";
	string scenePath = "..\\Resources\\Scenes";
	string toInclude = "#include \"pch.h\"\n#include \"DataManager.h\"\n#include \"Material.h\"\n#include \"Mesh.h\"\n#include \"Shader.h\"\n#include \"Component.h\"\n#include \"Texture.h\"\n";
	string toWrite = "";
	SearchingDir(assetPath, toInclude, toWrite, 'A');
	SearchingDir(scenePath, toInclude, toWrite, 'S');
	string fileFormat = toInclude + "\nDataManager::DataManager()\n{\n";
	fileFormat += toWrite;
	fileFormat += "}\n\nDataManager::~DataManager()\n{\n}\n";
	std::cout << fileFormat;

	std::ofstream ofs("../Engine/DataManager.cpp", std::ios::out | std::ios::trunc);
	if (ofs.fail())
	{
		std::cout << "File Open/Write Error!";
		return -1;
	}
	
	ofs << fileFormat;
	ofs.close();
}

void SearchingDir(string InPath, string& InToInclude, string& InToWrite, char InToParse)
{
	int checkDirFile = 0;
	string dirPath = InPath + "\\*.*";
	struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
	intptr_t handle;
	list<_finddata_t> fdlist;//���丮, ���� ���� ��ü ����Ʈ

	if ((handle = _findfirst(dirPath.c_str(), &fd)) == -1L) //fd ����ü �ʱ�ȭ.
	{
		//�����̳� ���丮�� ���� ���.
		cout << "No file in directory!" << endl;
		return;
	}

	if (InToParse == 'A')
	{
		do //���� Ž�� �ݺ� ���� (�� ����)
		{
			checkDirFile = IsFileOrDir(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
			if (checkDirFile == 0 && fd.name[0] != '.')
			{
				//���丮�� ���� ����
				if (string(fd.name).compare("Scenes"))
					continue;

				cout << "Dir  : " << InPath + "\\" << fd.name << endl;
				SearchingDir(InPath + "\\" + fd.name, InToInclude, InToWrite);//����� ȣ��
			}
			else if (checkDirFile == 1 && fd.name[0] != '.')
			{
				//������ ���� ����
				cout << "File : " << InPath + "\\" << fd.name << endl;
				fdlist.push_back(fd);
				ParseAssetFiles(InPath + "\\" + fd.name, InToInclude, InToWrite);
			}

		} while (_findnext(handle, &fd) == 0);
	}
	else if (InToParse == 'S')
	{
		do //���� Ž�� �ݺ� ���� (�� ����)
		{
			checkDirFile = IsFileOrDir(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
			if (checkDirFile == 0 && fd.name[0] != '.')
			{
				cout << "Dir  : " << InPath + "\\" << fd.name << endl;
				SearchingDir(InPath + "\\" + fd.name, InToInclude, InToWrite, InToParse);//����� ȣ��
			}
			else if (checkDirFile == 1 && fd.name[0] != '.')
			{
				//������ ���� ����
				cout << "File : " << InPath + "\\" << fd.name << endl;
				fdlist.push_back(fd);
				ParseSceneFiles(InPath + "\\" + fd.name, InToInclude, InToWrite);
			}

		} while (_findnext(handle, &fd) == 0);
	}
	_findclose(handle);
}


bool IsFileOrDir(_finddata_t fd)
//�������� ���丮���� �Ǻ�
{

	if (fd.attrib & _A_SUBDIR)
		return 0; // ���丮�� 0 ��ȯ
	else
		return 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

}

void ParseAssetFiles(string InPath, string& InToInclude, string& InFileWrite)
{
	//tinyxml2::XMLNode* root = sceneFile.FirstChild();
	//tinyxml2::XMLElement* object = root->FirstChildElement("Object");
	//for (tinyxml2::XMLElement* nextObj = object; nextObj != NULL; nextObj = nextObj->NextSiblingElement())
	//{

	//	uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));


	//	switch (static_cast<OBJECT_TYPE>(objType))
	//	{
	//	case OBJECT_TYPE::GAMEOBJECT:
	//	{

	//		break;
	//	}
	//	case OBJECT_TYPE::MATERIAL:
	//	{
	//		InFileWrite.append("\t_ObjectMap.insert(make_pair(");
	//		InFileWrite += nextObj->Attribute("FileID");
	//		InFileWrite.append(", make_pair(");
	//		InFileWrite += InPath;
	//		InFileWrite.append("));\n");
	//		break;
	//	}
	//	case OBJECT_TYPE::MESH:
	//	{
	//		InFileWrite.append("\t_ObjectMap.insert(make_pair(");
	//		InFileWrite += nextObj->Attribute("FileID");
	//		InFileWrite.append(", ");
	//		InFileWrite += InPath;
	//		InFileWrite.append("));\n");
	//		break;
	//	}
	//	case OBJECT_TYPE::SHADER:
	//	{

	//		break;
	//	}
	//	case OBJECT_TYPE::COMPONENT:
	//	{
	//		InToInclude.append("#include \"");
	//		InToInclude += nextObj->FirstChildElement("Component_Name")->GetText();
	//		InToInclude.append("\"\n");

	//		InFileWrite.append("\t_ComponentMap.insert(make_pair(");
	//		InFileWrite += nextObj->Attribute("FileID");
	//		InFileWrite.append(", new ");
	//		InFileWrite += nextObj->FirstChildElement("Component_Name")->GetText(); // Transform, MonoBehaviour etc.
	//		InFileWrite.append("()));\n");
	//		break;
	//	}
	//	case OBJECT_TYPE::SPRITE:
	//	{
	//		break;
	//	}
	//	case OBJECT_TYPE::TEXTURE:
	//	{
	//		InFileWrite.append("\t_ObjectMap.insert(make_pair(");
	//		InFileWrite += nextObj->Attribute("FileID");
	//		InFileWrite.append(", make_pair(");
	//		InFileWrite += nextObj->Attribute("Object_Type");
	//		InFileWrite.append(", new Texture())));\n");
	//		break;
	//	}
	//	}
	//}
	//
	//InFileWrite.append("_SceneMap.insert(");
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
