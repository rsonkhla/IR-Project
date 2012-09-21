/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef WIKI_PARSER
#define WIKI_PARSER

// include
#include "CommonSettingsAndUtilities.h"
#include "DataCollector.h"
#include "Dictionary.h"
#include "InvertedIndexHashMap.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#ifdef MY_WINDOWS
#else 
#endif

using namespace std;

/**
*
*/
class WikiParser
{
	//functions
public:
	WikiParser(string outputDirectory);
	~WikiParser();
	bool PreProcessDocument(Document &doc, Document &semWiki, bool dumpMetadata, vector<string> &authVector, vector<string> &catgVector);
	bool DumpDictionaries();
	bool DumpRemainingInvIndx();
private:
	bool DumpMetadata(string &dumpPath, string &data);
	bool ExtractMetadata(string docName, size_t docID, string &docData, string &semWikiData, vector<string> &authVector, vector<string> &catgVector);
	bool RemoveMarkupData(string &data);
	
	//data members 
public:
private:
	string m_sOutputDirectory;
	string m_sMetadata;
	string m_sMetadata_sw;
	string m_sLinkRepository;
	string m_sRefAndSectionCount;
	MyDictinary m_dAuthDict;
	MyDictinary m_dCatgDict;

public:
	// inverted indexes
	InvertedIndexHashMap m_AuthorInvIndx;
	InvertedIndexHashMap m_CategoryInvIndx;
};

#endif
