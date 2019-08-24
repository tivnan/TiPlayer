//
//����ʹ����֧������·��
//TCHAR��CHAR���ͻ���

#include "stdafx.h"
#include<stringapiset.h>
#include<string>

#ifndef _CHARASETTRAN_H
#define _CHARASETTRAN_H

//Unicode ת UTF-8
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;

	//���char�������賤��
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);

	//�����ڴ�������
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);

	//����
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

//Unicode ת UTF-8
void UnicodeToUtf8(CStringW& unicodeString, std::string& str)
{
	//��ȡ�������賤��
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	//�����ڴ�
	char* buffer = new char[stringLength + 1];

	//����
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;

}

//UTF-8תUnicode
wchar_t* Utf8ToUnicode(const char* utf8)
{
	int len = strlen(utf8) + 1;

	//���������ڴ��С��������
	wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * len);
	memset(temp, 0, len * sizeof(wchar_t));

	//����ת��
	MultiByteToWideChar(CP_ACP, 0, utf8, strlen(utf8), temp, len);

	return temp;
}

#endif 
