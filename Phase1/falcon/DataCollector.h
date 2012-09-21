/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef DATA_COLLECTOR
#define DATA_COLLECTOR

// include
#include "CommonSettingsAndUtilities.h"
#include "Dictionary.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#ifdef MY_WINDOWS
	//#include <Windows.h>
#else 
	#include <sys/types.h>
	#include <dirent.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif

using namespace std;

class Document
{
	// member functions
public:
	Document(string path, size_t docID, string data);
	~Document();
	void ClearData();
	// data members
public:
	string		m_sPath;
	size_t		m_iDocID;
	string		m_sData;
};

/**
* This class is used to scan directories to get a list of documents to be processed.
*/
class DataCollector
{
	// functions
public:
	DataCollector(string outputDirectory, size_t fileDictBarrelSize);
	~DataCollector();
	bool ScanForNewDocuments(string directoryPath);
	bool GetNextDocument(Document &doc);
	size_t AddNewDocumentToDict(size_t docLen, string path);
	void AddDocumentLengthToDict(size_t docLen);
	bool DumpFileDictionary();
	size_t GetNoOfDocs();
private:
	size_t GenerateUniqueDocID(string &docName);
	// data members
private:
	string m_sOutputDirectory;
	vector<string> m_vsFileNames;
	FileDictinary m_fileDict;
};

#endif
