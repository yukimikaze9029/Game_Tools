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
/// 创建多级目录
/// </summary>
/// <param name="szPath"></param>
/// <returns>
/// 返回：1——成功，0——失败， - 1——目录错误
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

