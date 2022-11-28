// DataManagerGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "EnginePch.h"

#include <io.h>
#include "tinyxml2.h"

void SearchingDir(string path, string& InToInclude, string& InToWrite);
bool IsFileOrDir(_finddata_t fd);
void ParseSceneFiles(string InPath, string& InToInclude, string& InFileWrite);

int main()
{
	string scenePath = "..\\Resources\\Scenes";
	string toInclude = "#include \"pch.h\"\n#include \"DataManager.h\"\n";
	string toWrite = "";
	SearchingDir(scenePath, toInclude, toWrite);
	string fileFormat = toInclude + "\nDataManager::DataManager()\n{\n}\n\nDataManager::~DataManager()\n{\n}\n\nvoid DataManager::Register()\n{\n";
	fileFormat += toWrite;
	fileFormat += "}\n";
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

void SearchingDir(string InPath, string& InToInclude, string& InToWrite)
{
	int checkDirFile = 0;
	string dirPath = InPath + "\\*.*";
	struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
	intptr_t handle;
	list<_finddata_t> fdlist;//디렉토리, 파일 정보 객체 리스트

	if ((handle = _findfirst(dirPath.c_str(), &fd)) == -1L) //fd 구조체 초기화.
	{
		//파일이나 디렉토리가 없을 경우.
		cout << "No file in directory!" << endl;
		return;
	}

	do //폴더 탐색 반복 시작
	{
		checkDirFile = IsFileOrDir(fd);//현재 객체 종류 확인(파일 or 디렉토리)
		if (checkDirFile == 0 && fd.name[0] != '.') {
			//디렉토리일 때의 로직
			cout << "Dir  : " << InPath + "\\" << fd.name << endl;
			SearchingDir(InPath + "\\" + fd.name, InToInclude, InToWrite);//재귀적 호출
		}
		else if (checkDirFile == 1 && fd.name[0] != '.') {
			//파일일 때의 로직
			cout << "File : " << InPath + "\\" << fd.name << endl;
			fdlist.push_back(fd);
			ParseSceneFiles(InPath + "\\" + fd.name, InToInclude, InToWrite);
		}

	} while (_findnext(handle, &fd) == 0);
	_findclose(handle);
}

bool IsFileOrDir(_finddata_t fd)
//파일인지 디렉토리인지 판별
{

	if (fd.attrib & _A_SUBDIR)
		return 0; // 디렉토리면 0 반환
	else
		return 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

}

void ParseSceneFiles(string InPath, string& InToInclude, string& InFileWrite)
{

	tinyxml2::XMLDocument sceneFile;
	tinyxml2::XMLError error = sceneFile.LoadFile(InPath.c_str());


	// Load scene information 
	tinyxml2::XMLNode* root = sceneFile.FirstChild();
	tinyxml2::XMLElement* object = root->FirstChildElement("Object");
	for (tinyxml2::XMLElement* nextObj = object; nextObj != NULL; nextObj = nextObj->NextSiblingElement())
	{

		uint8 objType = std::stoi(string(nextObj->Attribute("Object_Type")));

		switch (static_cast<OBJECT_TYPE>(objType))
		{
		case OBJECT_TYPE::GAMEOBJECT:
		{

			break;
		}
		case OBJECT_TYPE::MATERIAL:
		{

			break;
		}
		case OBJECT_TYPE::MESH:
		{

			break;
		}
		case OBJECT_TYPE::SHADER:
		{

			break;
		}
		case OBJECT_TYPE::COMPONENT:
		{
			InToInclude.append("#include \"");
			InToInclude += nextObj->FirstChildElement("Component_Type")->GetText();
			InToInclude.append("\"\n");

			InFileWrite.append("\t_ComponentMap.insert(make_pair(");
			InFileWrite += nextObj->Attribute("FileID");
			InFileWrite.append(", new ");
			InFileWrite += nextObj->FirstChildElement("Component_Type")->GetText(); // Transform, MonoBehaviour etc.
			InFileWrite.append("()));\n");
			break;
		}
		case OBJECT_TYPE::SPRITE:
		{
			break;
		}
		case OBJECT_TYPE::TEXTURE:
		{
			break;
		}
		}
	}
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
