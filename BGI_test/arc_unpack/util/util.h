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
