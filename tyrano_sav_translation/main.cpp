#define _CRT_SECURE_NO_WARNINGS
#include "URL_codec.h"
#include <fstream>
#include <pathcch.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "pathcch.lib")

int _tmain(int argc, _TCHAR** argv)
{
	URL_codec url;
	string strIn, strOut, buf;
	char ch;
	_TCHAR szPath[MAX_PATH], * pszExtension, szBackUp[MAX_PATH], szOutPath[MAX_PATH], * pszFileName;

	memset(szPath, '\0', MAX_PATH);
	memcpy(szPath, argv[1], 2 * lstrlen(argv[1]) + 1);
	memset(szBackUp, '\0', MAX_PATH);
	memcpy(szBackUp, argv[1], 2 * lstrlen(argv[1]) + 1);
	memset(szOutPath, '\0', MAX_PATH);
	memcpy(szOutPath, argv[0], 2 * lstrlen(argv[0]) + 1);
	PathCchRemoveFileSpec(szOutPath, sizeof(szOutPath));

	// open file
	std::ifstream in(szPath);
	while (!in.eof())
	{
		in >> buf;
		in.get(ch);
		strIn += buf;
		if (ch == ' ') strIn += ' ';
	}
	in.close();

	// back up file
	_tcscat(szBackUp, _T(".backup"));
	std::ofstream back_up(szBackUp);
	back_up << strIn;
	back_up.close();

	pszExtension = PathFindExtension(szPath);
	pszFileName = PathFindFileName(szPath);
	
	// switch decode or encode
	if (!_tcscmp(pszExtension, _T(".sav")))
	{
		strOut = url.URL_Decode(strIn);
		_tcscat(pszFileName, _T(".txt"));
		PathCchCombine(szOutPath, sizeof(szOutPath) / 2, szOutPath, pszFileName);
	}
	else if(!_tcscmp(pszExtension, _T(".txt")))
	{
		strOut = url.URL_Encode(strIn);
		PathCchRemoveExtension(szPath, sizeof(szPath));
		memcpy(szOutPath, szPath, 2 * lstrlen(szPath) + 1);
	}
	else
	{
		MessageBox(NULL, _T("暂不支持该扩展名"), _T("Warning"), MB_OK | MB_ICONASTERISK);
	}

	// save file
	std::ofstream out(szOutPath);
	out << strOut;
	out.close();

	return 0;
}