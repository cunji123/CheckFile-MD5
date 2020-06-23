#pragma once
#include "sta.h"
using namespace std;
typedef struct fileTree {
	bool flag;   //0为文件夹，1为文件
	string fileName;
	string vue_md5;
	fileTree* sun;
	fileTree* brother;
}fileTree;

fileTree* nNode(bool flag, string fileName, string vue_md5)
{
	fileTree* newNode = new fileTree;
	newNode->brother = newNode->sun = NULL;
	newNode->flag = flag;
	newNode->fileName = fileName;

	//如果为文件，则添加md5值
	if (flag)
		newNode->vue_md5 = vue_md5;
	return newNode;
}
void findFile(string dir, fileTree* father)
{
	fileTree* node = nNode(0, dir, "");      //插入文件夹节点
	if (father->sun == NULL)
		father->sun = node;
	else
	{
		fileTree* ptr = father->sun;
		while (ptr->brother != NULL)
			ptr = ptr->brother;
		ptr->brother = node;

	}
	WIN32_FIND_DATAA stFD;							//存放文件信息的结构体
	HANDLE h;
	string temp;

	temp = dir + "\\*";
	h = FindFirstFileA(temp.c_str(), &stFD);			//构建目录句柄



	while (FindNextFileA(h, &stFD))						//提取目录句柄对应目录所包含的文件
	{
		temp = dir + "\\" + stFD.cFileName;

		if (temp == dir + "\\..")										//上一级路径
		{
			continue;
		}
		else if (PathIsDirectoryA(temp.c_str()))		//包含子目录
		{
			findFile(temp, node);												//递归调用
		}
		else
		{
			string md5 = GetMd5(stringToLPCWSTR(temp));
			fileTree* nodeSun = nNode(1, temp, md5);      //插入文件节点
			if (node->sun == NULL)
				node->sun = nodeSun;
			else
			{
				fileTree* ptr = node->sun;
				while (ptr->brother != NULL)
					ptr = ptr->brother;
				ptr->brother = nodeSun;

			}
			//cout << stFD.cFileName << endl;				//打印文件名
		}
	}

	return;
}

void print(fileTree* node, int deep)
{
	for (int i = 0; i < deep; i++)
		cout << "|";
	cout << node->fileName;
	if (node->flag)
		cout << "  " << "MD5:" << node->vue_md5;
	cout << endl;

	fileTree* ptr = node->sun;
	while (ptr != NULL)
	{
		print(ptr, deep + 1);
		ptr = ptr->brother;
	}
}