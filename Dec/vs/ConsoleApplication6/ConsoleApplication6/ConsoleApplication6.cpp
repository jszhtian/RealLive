// ConsoleApplication6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef unsigned char byte;
typedef unsigned short word;
unsigned char* buf;
typedef unsigned int ulong;

void decompress_data(byte* comp_data, byte* decomp_begin, byte* decomp_end)
{
	byte* s = comp_data;
	byte* d = decomp_begin;
	byte c;
	while (true)
	{
		c = *s++;
		byte n = 8;
		do
		{
			if (d == decomp_end)
			{
				return;
			}
			if (c & 0x1)
			{
				memcpy(d, s, sizeof(byte));
				s++;
				d++;
			}
			else
			{
				unsigned int o, f;

				o = (unsigned int)*(unsigned short*)s;

				s += sizeof(unsigned short);
				f = o;
				o >>= 4;
				f = (f & 0xF) + 0x2;

				memcpy(d, (void*)(d - o), f);
				d += f;
			}
			c >>= 1;//右移计算
			n--;
		} while (n);
	}
}

int main()
{
	FILE* f = fopen("compress.dat", "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = (unsigned char*)malloc(size);
	fread(buf, size, 1, f);
	byte* decbuf = (byte*)malloc(0xA8CE);
	byte* bufend = decbuf + 0xA8CE;
	memset(decbuf, 0, 0xA8CE);
	decompress_data(buf, decbuf, bufend);
	FILE* f2 = _wfopen(L"output.txt", L"wb");
	fwrite(decbuf, 0xA8CE, 1, f2);
	fclose(f2);
	return 0;
}

