#ifndef TIME_ANALYSIS
#define TIME_ANALYSIS

#include <map>
#include <string>

class TimeStruct
{
public:
	TimeStruct();
	~TimeStruct();
public:
	size_t m_iStart;
	size_t m_iEnd;
};

class TimeAnalysis
{
public:
	TimeAnalysis();
	~TimeAnalysis();
public:
	//map<string, TimeStruct> m_timingInfo;
};

#endif
