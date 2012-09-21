#include "InvertedIndexGenerator.h"

InvertedIndexGenerator::InvertedIndexGenerator(string outputDirectory, size_t noOfBarrels, size_t tokensLimitPerBarrel)
{
	m_iNoOfBarrels			= noOfBarrels;
	m_tokensLimitPerBarrel	= tokensLimitPerBarrel;

	// initialize inverted index hash map for each barrel
	for(int i=0 ; i<m_iNoOfBarrels ; i++)
	{
		m_pIndex.push_back(new InvertedIndexHashMap(outputDirectory + "/index.inv.", i, tokensLimitPerBarrel));
	}
}

InvertedIndexGenerator::~InvertedIndexGenerator()
{
	// delete inverted index hash map for each barrel
	for(int i=0 ; i<m_iNoOfBarrels ; i++)
	{
		delete m_pIndex[i];
	}
}

int GetBarrelNo(string str, int noOfBarrels)
{
	int m, n, num;

	if(str.size() == 0)
		return 0;

	m	= str[0];

	if(str.size() > 1)
	{
		n	= str[1];
	}
	else
	{
		n	= 'a';
	}

	if(((m <= '9') && (m >= '0')) || (m == '-') )
	{
		m = 'a';
	}

	if(((n <= '9') && (n >= '0')) || (n == '-') )
	{
		n = 'a';
	}

	num = (int)((double)((m-97)*26 + (n-97)) / (double)676 * noOfBarrels);

	return num;
}

void InvertedIndexGenerator::AddData(size_t docId, vector<string> toks)
{
	vector<vector<string>*> splitToks;

	for(int i=0 ; i<m_iNoOfBarrels ; i++)
	{
		splitToks.push_back(new vector<string>);
	}

	for(vector<string>::iterator it = toks.begin() ; it != toks.end() ; it++)
	{
		//cout << GetBarrelNo(*it, m_iNoOfBarrels) << "\t" << *it << endl;

		(*(splitToks[GetBarrelNo(*it, m_iNoOfBarrels)])).push_back(*it);
	}

	for(int i=0 ; i<m_iNoOfBarrels ; i++)
	{
		(*m_pIndex[i]).AddData(docId, (*(splitToks[i])));
		delete (splitToks[i]);
	}
}

void InvertedIndexGenerator::DumpRemainingInvIndx()
{
	for(int i=0 ; i<m_iNoOfBarrels ; i++)
	{
		(*m_pIndex[i]).DumpInvertedIndex();
	}
}
