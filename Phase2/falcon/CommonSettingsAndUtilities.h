/*
Author		: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef COMMON_SETTINGS_AND_UTILITIES
#define COMMON_SETTINGS_AND_UTILITIES

//#define MY_WINDOWS // comment this statement while building on linux
#undef MY_WINDOWS
#define SEMI_WIKI_SNIPPET_CHAR_LENGTH 150

#include <string>

#ifdef MY_WINDOWS
	#include <Windows.h>
	#include <WinBase.h>
#else 
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <limits.h>
	#include <stdlib.h>
#endif

using namespace std;

bool CreateNewDirectory(string path);

#endif
