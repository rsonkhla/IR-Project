/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#include "CommonSettingsAndUtilities.h"


bool CreateNewDirectory(string path)
{
#ifdef MY_WINDOWS
	return CreateDirectory(path.c_str(), NULL);
#else
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	return false;
}