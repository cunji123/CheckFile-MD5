
#include "sta.h"
using namespace std;

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}
string GetMd5(LPCWSTR FileDirectory)
{
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)                                        //如果CreateFile调用失败
	{
		cout << "CreateFile go wrong :" << GetLastError() << endl;                //提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。
		CloseHandle(hFile);
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //获得CSP中一个密钥容器的句柄
	{
		cout << "CryptAcquireContext go wrong:" << GetLastError() << endl;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)     //初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被CryptHashData调用。
	{
		cout << "CryptCreateHash go wrong:" << GetLastError() << endl;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);    //获取文件的大小
	if (dwFileSize == 0xFFFFFFFF)               //如果获取文件大小失败
	{
		cout << "GetFileSize go wrong:" << GetLastError() << endl;
	}
	byte* lpReadFileBuffer = new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)        //读取文件
	{
		cout << "ReadFile go wrong:" << GetLastError() << endl;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)      //hash文件
	{
		cout << "CryptHashData go wrong:" << GetLastError() << endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //关闭文件句柄
	BYTE* pbHash;
	DWORD dwHashLen = sizeof(DWORD);
	//以下注释掉的代码不用使用，因为已经知道md5值就占32个字节，没有必要通过CryptGetHashParam函数来得到字节数。
	/*
	BYTE *pbHashSize;
	if (!(pbHashSize=(byte*)malloc(dwHashLen)))      //为pbHashSize分配内存
	{
		cout<<"memory allocation failed:"<<GetLastError()<<endl;
	}
	//将第二个参数的值设为HP_HASHSIZE。dwHashLen中存放着hash值的字节数。这个调用必须在将第三个参数设置为HP_HASHVAL的调用前，这样才能分配正确数量的内存。
	if (CryptGetHashParam(hHash,HP_HASHSIZE,pbHashSize,&dwHashLen,0))
	{
		free(pbHashSize);
	}
	else
	{
		cout<<"get size go wrong"<<GetLastError()<<endl;
	}*/
	if (CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0))      //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
	{
	}
	else
	{
		cout << "get length wrong" << endl;
	}
	if (pbHash = (byte*)malloc(dwHashLen))
	{
	}
	else
	{
		cout << "allocation failed" << endl;
	}
	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))            //获得md5值
	{
		string str = "";
		
		for (DWORD i = 0; i < dwHashLen; i++)         //输出md5值
		{
			if ((int)pbHash[i] / 16 < 10)
				str += (int)pbHash[i] / 16+48;
			else
				str += (int)pbHash[i] / 16 - 10 + 97;
			
			if ((int)pbHash[i] % 16 < 10)
				str += (int)pbHash[i] % 16+48;
			else
				str += (int)pbHash[i] % 16 - 10 + 97;

			//printf("%02x", pbHash[i]);	
		}
	
		return str;
	}
	//善后工作
	if (CryptDestroyHash(hHash) == FALSE)          //销毁hash对象
	{
		cout << "CryptDestroyHash go wrong:" << GetLastError() << endl;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		cout << "CryptReleaseContext go wrong:" << GetLastError() << endl;
	}
}


