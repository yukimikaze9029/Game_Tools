#define CRYPTO_UTIL
#include "util/util.h"
#include <fstream>

struct PFS_HEAD
{
	char magic[3];// pf
	char version;
	int indexSize;// data_offset = indexSize + 7
	int fileNum;// 
	PFS_HEAD() :version('\0'), indexSize(0), fileNum(0) { magic[2] = '\0'; };
};

struct PFS_FILE_INFO
{
	int filePathLenth;
	char *filePath;
	int unknown;// ¹Ì¶¨ 0x0 £¿
	int offset;
	int fileSize;
	~PFS_FILE_INFO()
	{
		delete[] filePath;
	}
};

struct PFS_FILE_INFO_2
{
	size_t offset;// PFS_FILE_INFO.unknown_offset = this->offset + 7
	size_t v8;// v8 = 0x0
};


int _tmain(int argc, _TCHAR** argv)
{
	if (argc < 2)
	{
		cout << "Please drag & drop .pfs file onto this exe" << endl;
		return 0;
	}

	_TCHAR* pszInFile = argv[1];
	wstring szStdPath;
	PFS_HEAD pfs_head;
	PFS_FILE_INFO* file_info;
	size_t offset_index_num;
	PFS_FILE_INFO_2* offset_index;
	size_t offset_index_start;
	BYTE* index, *data;
	string key;
	std::ifstream in;
	std::ofstream out;
	
	in.open(pszInFile, std::ios::binary);

	// read magic == "pf"
	in.read(pfs_head.magic, 2);
	if (strcmp(pfs_head.magic, "pf"))
	{
		cout << "not .pfs file" << endl;
		cout << "Press ENTER to exit" << endl;
		getchar();
		return 0;
	}
	// read version == 8
	in.read((char*)&pfs_head.version, 1);
	// read index_size
	in.read((char*)&pfs_head.indexSize, sizeof(int));

	auto index_start = in.tellg();
	auto index_end = index_start + (decltype(index_start))pfs_head.indexSize;

	// read file_num
	in.read((char*)&pfs_head.fileNum, sizeof(int));

	// read file_info
	file_info = new PFS_FILE_INFO[pfs_head.fileNum];

	for (int i = 0; i < pfs_head.fileNum; i++)
	{
		in.read((char*)&file_info[i].filePathLenth, sizeof(int));

		file_info[i].filePath = new char[file_info[i].filePathLenth + 1];
		in.read(file_info[i].filePath, file_info[i].filePathLenth);
		file_info[i].filePath[file_info[i].filePathLenth] = '\0';

		in.read((char*)&file_info[i].unknown, sizeof(int));
		in.read((char*)&file_info[i].offset, sizeof(int));
		in.read((char*)&file_info[i].fileSize, sizeof(int));
	}

	// read offset_index
	in.read((char*)&offset_index_num, sizeof(int));
	offset_index = new PFS_FILE_INFO_2[offset_index_num];

	for (size_t i = 0; i < offset_index_num; i++)
	{
		in.read((char*)&offset_index[i].offset, sizeof(int));
		in.read((char*)&offset_index[i].v8, sizeof(int));
	}
	in.read((char*)&offset_index_start, sizeof(int));

	// prepare key
	in.seekg(index_start);
	index = new BYTE[pfs_head.indexSize];
	in.read((char*)index, pfs_head.indexSize);
	SHA1encode(key, index, pfs_head.indexSize);

	auto pszEx = PathFindExtension(pszInFile);
	if (_tcscmp(pszEx, _T("pfs")))
	{
		PathRemoveExtension(pszInFile);
	}
	PathRemoveExtension(pszInFile);
	if (!PathIsDirectory(pszInFile))
	{
		CreateDirectory(pszInFile, NULL);
	}
	szStdPath = pszInFile;

	// read data
	for (int i = 0; i < pfs_head.fileNum; i++)
	{
		int n = MultiByteToWideChar(CP_932, 0, file_info[i].filePath, -1, NULL, 0) + 2;
		_TCHAR* pszFilePath = new _TCHAR[n];
		MultiByteToWideChar(CP_932, 0, file_info[i].filePath, -1, pszFilePath, n - 2);

		printf("Extracting file [ %d / %d ] : %ls", i + 1, pfs_head.fileNum, pszFilePath);
		cout << endl;

		data = new BYTE[file_info[i].fileSize];

		in.seekg(file_info[i].offset);
		
		in.read((char*)data, file_info[i].fileSize);

		// decode
		for (int j = 0; j < file_info[i].fileSize; j++)
		{
			data[j] ^= key[j % key.length()];
		}

		wstring szOutPath = szStdPath + _T("\\") + pszFilePath;

		if (!CreatPath(szOutPath))
		{
			MessageBox(NULL, _T("Create Dir Faild"), _T("Error"), MB_OK | MB_ICONERROR);
			cout << "Press ENTER to exit" << endl;
			getchar();
			return 1;
		}

		out.open(szOutPath, std::ios::binary);
		out.write((char*)data, file_info[i].fileSize);
		out.close();

		delete[] pszFilePath;
		delete[] data;
	}
	delete[] file_info;
	delete[] offset_index;

	cout << "Extracting over" << endl;
	cout << "Press ENTER to exit" << endl;
	getchar();
	return 0;
}