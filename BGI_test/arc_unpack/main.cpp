#include "util\util.h"
#include <fstream>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define CP_932 932

struct ARC_HEAD
{
	char magic[12];// BURIKO ARC20
	int file_num;
};

struct FILE_INFO {
	char file_name[96];
	int offset;
	int file_size;
	int unknown1[4];
	int is_raw_data;
	int unknown2;
};


int _tmain(int argc, _TCHAR** argv)
{
	if (argc < 2)
	{
		cout << "Please drag & drop .arc file onto this exe" << endl;
		return 0;
	}

	_TCHAR* pszInFile = argv[1];
	wstring szOutPath;
	ARC_HEAD arc_head;
	FILE_INFO* file_info;
	BYTE* data;
	std::ifstream in;
	std::ofstream out;

	in.open(pszInFile, std::ios::binary);
	in.read(arc_head.magic, sizeof(arc_head.magic));
	in.read((char*)&arc_head.file_num, sizeof(int));

	file_info = new FILE_INFO[arc_head.file_num];

	cout << "file num: " << arc_head.file_num << endl;

	for (int i = 0; i < arc_head.file_num; i++)
	{
		in.read(file_info[i].file_name, sizeof(file_info[i].file_name));
		in.read((char*)&file_info[i].offset, sizeof(int));
		in.read((char*)&file_info[i].file_size, sizeof(int));
		in.read((char*)&file_info[i].unknown1, sizeof(file_info[i].unknown1));
		in.read((char*)&file_info[i].is_raw_data, sizeof(int));
		in.read((char*)&file_info[i].unknown2, sizeof(int));
	}

	PathRemoveExtension(pszInFile);
	if (!PathIsDirectory(pszInFile))
	{
		CreateDirectory(pszInFile, NULL);
	}
	szOutPath = pszInFile;

	for (int i = 0; i < arc_head.file_num; i++)
	{	
		int n = MultiByteToWideChar(CP_932, 0, file_info[i].file_name, -1, NULL, 0) + 2;
		_TCHAR* pszFileName = new _TCHAR[n];
		MultiByteToWideChar(CP_932, 0, file_info[i].file_name, -1, pszFileName, n - 2);

		printf("Extracting file [ %d / %d ] : %ls", i + 1, arc_head.file_num, pszFileName);
		cout << endl;

		data = new BYTE[file_info[i].file_size];
		in.read((char*)data, file_info[i].file_size);
		out.open(szOutPath + _T("\\") + pszFileName, std::ios::binary);
		out.write((char*)data, file_info[i].file_size);
		out.close();
		delete[] pszFileName;
		delete[] data;
	}

	delete[] file_info;

	in.close();

	cout << "Extract success" << endl;
	
	return 0;
}