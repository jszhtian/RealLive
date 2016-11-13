// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;


int main(int argc, char *argv[])
{
	const int MAX = 65536;
	string addide = "Text Address:";
	string allocide = "Allocation Size:";
	string textide = "Text:";




	ofstream flog("Report.log");
	

	if (argc == 1)
	{
		cout << "Usage:" << argv[0] << " TxtFile"<< endl;
		return 0;
	}
	cout << "Only for Text cleanup!" << endl;
	ifstream fin(argv[1]);
	string tmp = argv[1];
	tmp.replace(tmp.size() - 4, tmp.size(), "out.txt");
	ofstream fout(tmp);
	if (fin.fail())
	{
		cout << "Open File Fail!" << endl;
		flog << "Open File Fail!" << endl;
		return -1;
	}
	string buf;
	while (getline(fin,buf))
	{
		int checkflag1 = 0;
		//cout << "Read From File:" <<buf<<endl;
		checkflag1 = buf.find(addide);
		if (checkflag1==buf.npos)
		{
			goto startnew;
		}
		int addressoffset = 0;
		int addressend = 0;
		addressend = buf.find(allocide);
		char address[128] = { 0 };
		buf.copy(address, addressend - addressoffset - addide.length() - 3, addressoffset + addide.length() + 2);
		address[addressend - addressoffset - addide.length() - 1] = '\0';
		int Adds = 0;
		Adds = strtol(address, NULL, 16);
		int TextAllo = 0;
		TextAllo = buf.find(textide);
		char Allo[128] = { 0 };
		buf.copy(Allo, TextAllo - 1 - addressend - allocide.length(), addressend + allocide.length());
		int tex = 0;
		tex = strtol(Allo, NULL, 16);
		if (tex%2==0)
		{
			char check = { 0 };
			buf.copy(&check, 1, TextAllo + textide.length());
			if (buf.find("_", TextAllo + textide.length()) != buf.npos)
			{
				goto startnew;
			}
			try
			{
				if (isalnum(check))
				{
					goto startnew;
				}
			}
			catch (exception ex)
			{

			}
		



			fout << "+" << buf << endl;
			fout << "-" << buf << endl;
			fout << endl;



		}
		
	startnew:Sleep(1);
	}
		
	return 0;
}

