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
//�������� ���丮���� �Ǻ�
{

	if (fd.attrib & _A_SUBDIR)
		return false; // ���丮�� 0 ��ȯ
	else
		return true; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ

}

bool DirectoryExplorer::SearchingDir(string InPath)
{
	int checkDirFile = 0;
	string dirPath = InPath + "\\*.*";
	struct _finddata_t fd;//���丮 �� ���� �� ���� ���� ���� ��ü
	intptr_t handle;
	list<_finddata_t> fdlist;//���丮, ���� ���� ��ü ����Ʈ

	if ((handle = _findfirst(dirPath.c_str(), &fd)) == -1L) //fd ����ü �ʱ�ȭ.
	{
		//�����̳� ���丮�� ���� ���.
		return false;
	}


	do //���� Ž�� �ݺ� ���� (�� ����)
	{
		checkDirFile = IsFileOrDir(fd);//���� ��ü ���� Ȯ��(���� or ���丮)
		if (checkDirFile == 0 && fd.name[0] != '.')
		{
			//���丮�� ���� ����
			SearchingDir(InPath + "\\" + fd.name, InToInclude, InToWrite);//����� ȣ��
		}
		else if (checkDirFile == 1 && fd.name[0] != '.')
		{
			//������ ���� ����
			fdlist.push_back(fd);
			ParseAssetFiles(InPath + "\\" + fd.name, InToInclude, InToWrite);
		}

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);
	return true;
}
