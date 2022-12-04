#pragma once
#include <fstream>
#include "EnginePch.h"

#include <io.h>

class DirectoryExplorer
{
	bool IsFileOrDir(_finddata_t fd);
	bool SearchingDir(string path);
	void ParseAssetFiles(string InPath, string& InToInclude, string& InFileWrite);
};

bool DirectoryExplorer::IsFileOrDir(_finddata_t fd)
//파일인지 디렉토리인지 판별
{

	if (fd.attrib & _A_SUBDIR)
		return false; // 디렉토리면 0 반환
	else
		return true; // 그밖의 경우는 "존재하는 파일"이기에 1 반환

}

bool DirectoryExplorer::SearchingDir(string InPath)
{
	int checkDirFile = 0;
	string dirPath = InPath + "\\*.*";
	struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
	intptr_t handle;
	list<_finddata_t> fdlist;//디렉토리, 파일 정보 객체 리스트

	if ((handle = _findfirst(dirPath.c_str(), &fd)) == -1L) //fd 구조체 초기화.
	{
		//파일이나 디렉토리가 없을 경우.
		return false;
	}


	do //폴더 탐색 반복 시작 (씬 제외)
	{
		checkDirFile = IsFileOrDir(fd);//현재 객체 종류 확인(파일 or 디렉토리)
		if (checkDirFile == 0 && fd.name[0] != '.')
		{
			//디렉토리일 때의 로직
			SearchingDir(InPath + "\\" + fd.name, InToInclude, InToWrite);//재귀적 호출
		}
		else if (checkDirFile == 1 && fd.name[0] != '.')
		{
			//파일일 때의 로직
			fdlist.push_back(fd);
			ParseAssetFiles(InPath + "\\" + fd.name, InToInclude, InToWrite);
		}

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
	return true;
}
