/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#ifndef MY_DICTIONARY
#define MY_DICTIONARY

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

class MyDictinary
{
	// member functions
public:
	MyDictinary();
	~MyDictinary();

	bool AddNewElementIfNotPresent(string key);
	void DumpDictionary(string path);
private:
	// data members
public:
	map<string, size_t> m_mDict;
	size_t m_iIDCounter;
private:
	map<string, size_t>::iterator m_it;
};

class BarrelOffsetPair
{
public:
	BarrelOffsetPair();
	~BarrelOffsetPair();

public:
	size_t m_iDocLen;
	size_t m_iBarrelNo;
	size_t m_iOffset;
};

class FileDictinary
{
	// member functions
public:
	FileDictinary(string outputPath, size_t barrelLimit);
	~FileDictinary();

	size_t AddNewElement(size_t docLen, string key);	// return id
	void DumpDictionary();
	void DumpFileNameBarrel();
private:
	// data members
public:
private:
	map<size_t, BarrelOffsetPair> m_mDict;
	size_t m_iIDCounter;

	string m_sOutputPath;
	size_t m_iBarrelNo;
	size_t m_iBarrelLimit;

	string m_sKeysLog;
};

#endif