/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#include "DataTypeDetector.h"

DataTypeDetector::DataTypeDetector()
{
}

DataTypeDetector::~DataTypeDetector()
{
}

/**
*	Detect type based on document path.
*/
DATA_TYPE DataTypeDetector::GetDataType(string docPath)
{
	string::size_type loc	= string::npos;
	string sSrchStr1		= "/news";
	string sSrchStr2		= "/wiki";

	loc	= docPath.find(sSrchStr1, 0);
	if(loc != string::npos)
	{
		return T_NEWS;
	}
	
	loc	= docPath.find(sSrchStr2, 0);
	if(loc != string::npos)
	{
		return T_WIKI;
	}

	return T_DEFAULT;
}
