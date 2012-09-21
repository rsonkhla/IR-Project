#ifndef INVERTED_INDEX_GENERATOR
#define INVERTED_INDEX_GENERATOR

#include "InvertedIndexHashMap.h"

class InvertedIndexGenerator
{
	// member functions
public:
	InvertedIndexGenerator(string outputDirectory, size_t noOfBarrels, size_t tokensLimitPerBarrel);
	~InvertedIndexGenerator();
	
	void AddData(size_t docId, vector<string> toks);
	void DumpRemainingInvIndx();

private:
	vector<InvertedIndexHashMap*> m_pIndex;
	size_t m_iNoOfBarrels;
	size_t m_tokensLimitPerBarrel;
};

#endif