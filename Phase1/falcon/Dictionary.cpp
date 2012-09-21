/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#include "Dictionary.h"

MyDictinary::MyDictinary()
{
	m_iIDCounter	= 0;
}

MyDictinary::~MyDictinary()
{
}

bool MyDictinary::AddNewElementIfNotPresent(string key)
{
	m_it	= m_mDict.find(key);
	if(m_it != m_mDict.end())
	{
		// if this key already exists then do nothing	
		return false;
	}
	else
	{
		m_mDict[key] = m_iIDCounter++;
		return true;
	}
}

void MyDictinary::DumpDictionary(string path)
{
	ofstream dictDump;
	dictDump.open(path.c_str());
	if (!dictDump) 
	{
		cout << "Error : Can not open file for dumping dictionary." << endl;
	}

	for(map<string, size_t>::iterator it = m_mDict.begin() ; it != m_mDict.end() ; it++)
	{
		dictDump << (*it).first << "," << (*it).second << endl;
	}

	dictDump.close();
}

// BarrelOffsetPair

BarrelOffsetPair::BarrelOffsetPair()
{
	m_iBarrelNo	= 0;
	m_iOffset	= 0;
}

BarrelOffsetPair::~BarrelOffsetPair()
{
}

// FileDictionary

FileDictinary::FileDictinary(string outputPath, size_t barrelLimit):
m_sOutputPath(outputPath), m_iBarrelLimit(barrelLimit)
{
	m_iIDCounter		= 0;
	m_iBarrelNo			= 0;
}

FileDictinary::~FileDictinary()
{
}

size_t FileDictinary::AddNewElement(size_t docLen, string key)
{
	size_t fileId		= m_iBarrelLimit*m_iBarrelNo + m_iIDCounter++;

	BarrelOffsetPair tempPair;
	tempPair.m_iDocLen		= docLen;
	tempPair.m_iBarrelNo	= m_iBarrelNo;
	tempPair.m_iOffset		= m_sKeysLog.size();
	m_mDict[fileId]			= tempPair;

	m_sKeysLog.append(key + "$");

	if(m_iIDCounter > m_iBarrelLimit)
	{
		// dump dict
		DumpDictionary();
		// update data
		m_iIDCounter	= 0;	// for phase 2
		m_iBarrelNo++;		// for phase 2
		m_sKeysLog.clear();
	}

	return fileId;
}

void FileDictinary::DumpFileNameBarrel()
{
	stringstream fp;
	fp << m_iBarrelNo;
	fp << "_FileNames.txt";

	string tempStr;
	tempStr	= m_sOutputPath + "/";
	tempStr.append(fp.str());

	ofstream dictDump;
	dictDump.open(tempStr.c_str());
	if (!dictDump) 
	{
		cout << "Error : Can not open file for dumping file dictionary." << endl;
	}
	dictDump << m_sKeysLog;
	dictDump.close();
}

void FileDictinary::DumpDictionary()
{
	stringstream fp;
	fp << m_iBarrelNo;
	fp << "_FileDict.txt";

	string tempStr;
	tempStr	= m_sOutputPath + "/";
	tempStr.append(fp.str());

	ofstream dictDump;
	dictDump.open(tempStr.c_str(), std::ios_base::app);
	if (!dictDump) 
	{
		cout << "Error : Can not open file for dumping file dictionary." << endl;
	}

	BarrelOffsetPair tempPair;
	for(map<size_t, BarrelOffsetPair>::iterator it = m_mDict.begin() ; it != m_mDict.end() ; it++)
	{
		BarrelOffsetPair &tempPair	= (*it).second;

		stringstream tempPairString;
		tempPairString << tempPair.m_iDocLen << ":" << tempPair.m_iBarrelNo << ":" << tempPair.m_iOffset;
		
		dictDump << (*it).first << "," << tempPairString.str() << endl;
	}

	dictDump.close();	
}