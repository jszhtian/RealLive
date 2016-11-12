// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#pragma warning(disable:4311)
#pragma warning(disable:4312)  

typedef unsigned char byte;
typedef unsigned short word;
unsigned char* buf;
typedef unsigned int ulong;
using namespace std;
//static int flag = 1;
char* text;
typedef HANDLE(WINAPI* fnCreateFileW)(LPCTSTR filename, DWORD acc, DWORD smo, LPSECURITY_ATTRIBUTES lpsec, DWORD credis, DWORD flags, HANDLE htmpf);
fnCreateFileW pcreatefile;
ofstream flog("Report.log");
void NumberPatch() {}
#define HOOK_BYTES 5  
typedef unsigned int uint;

uint hookAddr = 0;
char old_opcode[HOOK_BYTES];
char new_opcode[HOOK_BYTES];
char target_opecode[HOOK_BYTES] = {0x90,0x90,0x90,0x90,0x90};

struct FileInfo
{
	int isFile;
	string filename;
}targetfile;



char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}


HANDLE WINAPI newcreatefile(LPCTSTR filename, DWORD acc, DWORD smo, LPSECURITY_ATTRIBUTES lpsec, DWORD credis, DWORD flags, HANDLE htmpf)
{
	time_t nowtime = time(NULL);
	//flog << "CreateFile Get!" << endl;
	//flog << ctime(&nowtime)<<"Flag:"<<flag<<endl;
	text = wtoc(filename);
	string textstore;
	textstore = text;
	transform(textstore.begin(), textstore.end(), textstore.begin(), ::tolower);
	int pos1;
	int pos2;
	pos1=textstore.find("gameexe.dat", 0);
	pos2 = textstore.find("scene.pck", 0);
	if (pos1!=textstore.npos||pos2!=textstore.npos)
	{
//		flag = 1;
		flog << ctime(&nowtime);
		targetfile.filename = textstore;
		targetfile.isFile = 1;
		//flog << "File Name:" << textstore << endl << "Flag=" << flag << endl << endl;
		//flog <<endl<< "File Name:" << textstore << endl << "Flag=" << targetfile.isFile << endl << endl;
		flog<<endl<< "Struct info!" << " Filename:" << targetfile.filename << " Flaginfo:" << targetfile.isFile << endl;

	}
	else
	{
//		flag = 0;
		targetfile.isFile = 0;
	}

	return pcreatefile(filename, acc, smo, lpsec, credis, flags, htmpf);
}

bool hook(void)
{
	DWORD dwflag;
	if (VirtualProtect((void*)hookAddr,HOOK_BYTES,PAGE_EXECUTE_READWRITE,&dwflag))
	{
		memcpy(old_opcode, (void*)hookAddr, HOOK_BYTES);
		if (memcmp(old_opcode,target_opecode,HOOK_BYTES)!=0)
		{
			flog << "Hook Target Error!" << endl << "Expect Target:" << target_opecode << endl << "Now Target:" << old_opcode << endl;
			return false;
		}
		memcpy((void*)hookAddr, new_opcode, HOOK_BYTES);
		VirtualProtect((void*)hookAddr, HOOK_BYTES, dwflag, &dwflag);
		return true;
	}
	return false;
}

void unhook(void)
{
	DWORD dwflag;
	if (VirtualProtect((void*)hookAddr, HOOK_BYTES, PAGE_EXECUTE_READWRITE, &dwflag))
	{
		memcpy((void*)hookAddr, old_opcode, HOOK_BYTES);
		VirtualProtect((void*)hookAddr, HOOK_BYTES, dwflag, &dwflag);
	}
}

void __declspec(naked) hook_jmp(void)
{
	__asm
	{
		//mov eax,flag
		mov ebx,offset targetfile
		mov eax,[ebx]targetfile.isFile
		ret
	}
}

void sethookbytes(uint addr)
{
	hookAddr = addr;
	new_opcode[0] = (char)0xE8;
	(uint&)new_opcode[1] = (uint)hook_jmp - addr - 5;
}


void BeginDetour()
{
	HANDLE modaddr = GetModuleHandle(L"SiglusEngine Patch5.exe");
	flog << "Now EXE Handle:" << modaddr << endl;
	pcreatefile = (fnCreateFileW)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "CreateFileW");
	DetourTransactionBegin();
	DetourAttach((void**)&pcreatefile, newcreatefile);
	DetourTransactionCommit();

	uint hookAddr = (uint)modaddr+0x555ad6;
	flog << "Target Code in:" << hookAddr;
	sethookbytes(hookAddr);
	if (hook())
	{
		flog << "Hook Set!" << endl;
	}
	else
	{
		flog << "Hook Set Fail!" << endl;
	}

	//pCreateFontIndirectW = (fnCreateFontIndirectW)GetProcAddress(GetModuleHandle("gdi32.dll"), "CreateFontIndirectW");
	//DetourTransactionBegin();
	//DetourAttach((void**)&pCreateFontIndirectW, newCreateFontIndirectW);
	//DetourTransactionCommit();


	//pg00_enter_reader = (void(__cdecl *)(void))0x0055F150;
	//pg00_leave_reader = (void(__cdecl *)(void))0x0055F3B4;


	//DetourTransactionBegin();
	//DetourAttach((void**)&pg00_set_indexes, g00_set_indexes);
	//DetourTransactionCommit();

	//DetourTransactionBegin();
	//DetourAttach((void**)&pg00_copy_rect, g00_copy_rect);
	//DetourTransactionCommit();




}



BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		BeginDetour();
		flog << "Hook Start!" << endl;
		break;
	default:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}
