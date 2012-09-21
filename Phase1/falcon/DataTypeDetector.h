/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef DATA_TYPE_DETECTOR
#define DATA_TYPE_DETECTOR

// include
#include "CommonSettingsAndUtilities.h"

#include <iostream>

#ifdef MY_WINDOWS
#else 
#endif

using namespace std;

/**
*	Type of data that is currently supported. 
*/
enum DATA_TYPE {T_DEFAULT, T_NEWS, T_WIKI};

/**
*	This class detect the type of data present in a document.
*	Data parsing depends on the data type.
*/

class DataTypeDetector
{
	// public functions
public:
	DataTypeDetector();
	~DataTypeDetector();

	DATA_TYPE GetDataType(string docPath);
};

#endif