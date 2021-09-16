#include "util.h"

void printVectorInt(vector<int>& vec)
{
    vector<int>::iterator it;
    for (it = vec.begin(); it != vec.end(); it++)
    {
        cout << *it << "\t";
    }
    cout << endl;
}


/// <summary>
/// �����༶Ŀ¼
/// </summary>
/// <param name="szPath"></param>
/// <returns>
/// ���أ�1�����ɹ���0����ʧ�ܣ� - 1����Ŀ¼����
/// </returns>
int CreatPath(const wstring& szPath)
{
	size_t nPathLength = szPath.length();
	if (nPathLength == 0 || nPathLength > MAX_PATH) return -1;
	wchar_t szTempPath[MAX_PATH] = { 0 };
	for (size_t i = 0; i < nPathLength; i++)
	{
		szTempPath[i] = szPath[i];
		if (szTempPath[i] == '\\')
		{
			if (!PathIsDirectory(szTempPath))
			{
				if (!CreateDirectory(szTempPath, NULL))
				{
					printf("mkdir error : \"%ls\"\nerror code : %d\n", szTempPath, GetLastError());
					return 0;
				}
			}
		}
	}
	return 1;
}

