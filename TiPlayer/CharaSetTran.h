//
//用于使程序支持中文路径
//TCHAR与CHAR类型互换

#include "stdafx.h"
#include<stringapiset.h>
#include<string>

#ifndef _CHARASETTRAN_H
#define _CHARASETTRAN_H

//Unicode 转 UTF-8
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;

	//获得char数组所需长度
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);

	//分配内存且置零
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);

	//编码
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

//Unicode 转 UTF-8
void UnicodeToUtf8(CStringW& unicodeString, std::string& str)
{
	//获取数组所需长度
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	//分配内存
	char* buffer = new char[stringLength + 1];

	//编码
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;

}

//UTF-8转Unicode
wchar_t* Utf8ToUnicode(const char* utf8)
{
	int len = strlen(utf8) + 1;

	//分配所需内存大小并且置零
	wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * len);
	memset(temp, 0, len * sizeof(wchar_t));

	//编码转换
	MultiByteToWideChar(CP_ACP, 0, utf8, strlen(utf8), temp, len);

	return temp;
}

#endif 
