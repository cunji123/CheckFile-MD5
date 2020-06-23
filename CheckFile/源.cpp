#include "md5.h"
#include "fileTree.h"
using namespace std;
/* 
    目录及子目录下，
	     不能有中文文件夹及文件！
		 不能有中文文件夹及文件！
		 不能有中文文件夹及文件！
*/

void compare(fileTree* father1,fileTree* father2)
{
	fileTree* ptr1 = father1->sun;
	fileTree* ptr2;
	while (ptr1 != NULL)
	{
		bool ifFound = 0;
		ptr2 = father2->sun;
		while (ptr2 != NULL)
		{
			if (ptr1->fileName == ptr2->fileName)
			{
				ifFound = 1;
				break;
			}
			ptr2 = ptr2->brother;
		}

		if (ifFound)
		{
			if(PathIsDirectoryA(ptr1->fileName.c_str()))//如果是文件夹，则递归
			{
				compare(ptr1, ptr2);
			}
			else         //如果是文件，则比较md5值
			{
				if (ptr1->vue_md5 != ptr2->vue_md5)
					cout << "修改点：" << ptr1->fileName << "-----" << ptr2->fileName << endl;
				
			}
		}
		else
		{
			cout << "删除处:" << ptr1->fileName << endl;
		}

		ptr1 = ptr1->brother;
	}

	ptr2 = father2->sun;
	while (ptr2 != NULL)
	{
		bool ifFound = 0;
		ptr1 = father1->sun;
		while (ptr1 != NULL)
		{
			if (ptr1->fileName == ptr2->fileName)
			{
				ifFound = 1;
				break;
			}
			ptr1 = ptr1->brother;
		}

		if (!ifFound)
			cout << "增加处:" << ptr2->fileName << endl;


		ptr2 = ptr2->brother;
	}
}
void main()
{
	string filePath;
	cout << "文件夹:" << endl;
	cin >> filePath;
	
	fileTree* root1,*root2;

	root1 = nNode(0, "root", "");
	findFile(filePath, root1);

	cout << "输入任意字符以开始比较..." << endl;
	int temp;
	cin >> temp;
	root2 = nNode(0, "root", "");
	findFile(filePath, root2);
	compare(root1, root2);
	
	
	system("pause");
}