#ifndef INVERTED_INDEX_HASH_MAP
#define INVERTED_INDEX_HASH_MAP

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#include "HashMapImplementation.h"

// InvertedIndexHashMap
class InvertedIndexValue
{
	// member fucntions
public:
	InvertedIndexValue();
	~InvertedIndexValue();

	// data members
public:
	size_t m_iDocFreq;
	size_t m_iPstngLen;
	string m_sPstngLst;
};

class InvertedIndexHashMap
{
	// member functions
public:
	InvertedIndexHashMap();
	InvertedIndexHashMap(string outputDirectory, size_t barrelNo, size_t tokensLimitPerBarrel);
	~InvertedIndexHashMap();
	void SetDataMembers(string outputDirectory, size_t barrelNo, size_t tokensLimitPerBarrel);

	void AddData(size_t docId, vector<string> toks);
	void DumpInvertedIndex();
	// data members
private:
	string m_sOutputPath;
	size_t m_iBarrelNo;
	size_t m_iPartitionNo;
	size_t m_tokensLimitPerBarrel;
	size_t m_noOfTokensProcessed;	
public:
	HashMapImplementation<InvertedIndexValue> m_invIndx;
	HashMapImplementation<int> m_docTermFreq;
};

#endif
