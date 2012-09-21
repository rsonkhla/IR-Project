/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef COMMON_SETTINGS_AND_UTILITIES
#define COMMON_SETTINGS_AND_UTILITIES

//#define MY_WINDOWS // comment this statement while building on linux
#undef MY_WINDOWS

#define NO_OF_POSTIONG_PER_BARREL 50000
#define NO_OF_FILE_PER_BARREL UINT_MAX
#define NO_OF_AUTHOR_INV_INDX_BARRELS 0
#define NO_OF_AUTHORS_PER_BARREL UINT_MAX
#define NO_OF_CATEGORY_INV_INDX_BARRELS 0
#define NO_OF_CATEGORIES_PER_BARREL UINT_MAX

#include <string>

#ifdef MY_WINDOWS
	#include <Windows.h>
	#include <WinBase.h>
#else 
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <limits.h>
#endif

using namespace std;

bool CreateNewDirectory(string path);

#endif
