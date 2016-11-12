// SLLoader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "detours.h"
//#define MAX_PATH  256

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}



int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	wchar_t* DirPath = new wchar_t[MAX_PATH];
	char* DLLPath = new char[MAX_PATH]; //testdll.dll
	//wchar_t* DetourPath = new wchar_t[MAX_PATH]; //detoured.dll
	wchar_t* ExePath = new wchar_t[MAX_PATH];//exefile
	GetCurrentDirectory(MAX_PATH, DirPath);
	CString Dir=DirPath;
	CString DLL= DirPath;
	DLL.Insert(DLL.GetLength(), L"\\SLEPatch.dll");
	//CString Detour= DirPath;
	//Detour.Insert(Detour.GetLength(), L"detoured.dll");
	CString Exe = DirPath;
	Exe.Insert(Exe.GetLength(), L"\\SiglusEngine Patch5.exe");
	
	//DetourPath = Detour.AllocSysString();
	ExePath = Exe.AllocSysString();
	DLLPath = wtoc(DLL.AllocSysString());
	std::cout << "Target EXE:" << wtoc(Exe.AllocSysString())<<std::endl << "Target DLL:" << wtoc(DLL.AllocSysString())<<std::endl;
	system("pause");
	DetourCreateProcessWithDll(ExePath,NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi,DLLPath,0);
	return 0;
}

