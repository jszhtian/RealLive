// ConsoleApplication7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
unsigned char* buf;


int main()
{
	HANDLE hFile = CreateFileW(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		DWORD nFileSize = ::GetFileSize(hFile, NULL);
		char* lpbuff = new char(nFileSize);
		memset(lpbuff, 0, nFileSize);
		DWORD nNumberofbytesRead;
		bool bret = ::ReadFile(hFile, lpbuff, nFileSize, &nNumberofbytesRead, NULL);
		if (bret)
		{
			printf("Out:%s", lpbuff);
		}

		CloseHandle(hFile);
	}
	system("pause");


    return 0;
}

