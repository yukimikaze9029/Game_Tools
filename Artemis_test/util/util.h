#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <windows.h>
#include <tchar.h>

#define CP_932 932

#ifndef STL_UTIL
#define STL_UTIL

#include <string>
using std::string;
using std::wstring;

#include <list>
using std::list;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

#include <set>
using std::set;

#include <algorithm>
using std::remove;
using std::remove_copy;
using std::remove_if;
using std::remove_copy_if;


void printVectorInt(vector<int>&);


#endif // !STL_UTIL


#ifndef PTR_UTIL
#define PTR_UTIL

#include <memory>
using std::unique_ptr;
using std::make_unique;


#endif // !PTR_UTIL


#ifndef PATH_UTIL
#define PATH_UTIL

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <PathCch.h>
#pragma comment(lib, "pathcch.lib")

/// <summary>
/// 创建多级目录
/// </summary>
/// <param name="szPath"></param>
/// <returns>
/// 返回：1——成功，0——失败， - 1——目录错误
/// </returns>
int CreatPath(const wstring& szPath);

#endif // !PATH_UTIL


#ifdef CRYPTO_UTIL

#include <sha.h>
#include <hex.h>
#include <files.h>
#pragma comment(lib, "cryptlib.lib")
using namespace CryptoPP;

/// <summary>
/// 计算 SHA1
/// </summary>
/// <param name="digestStr"> 输出摘要 </param>
/// <param name="originStr"> 输入字节流 </param>
/// <param name="originLen"> 输入字节流长度 </param>
/// <returns> 摘要长度 </returns>
size_t SHA1encode(string& digestStr, const BYTE* originStr, size_t originLen)
{
	size_t size;
	SHA1 sha;
	sha.Update(originStr, originLen);
	size = sha.DigestSize();
	digestStr.resize(size);
	sha.Final((byte*)&digestStr[0]);

	return size;
}

#endif // !CRYPTO_UTIL
