#ifndef DICT_HASH_MAP
#define DICT_HASH_MAP

#include <string>
#include <queue>

using namespace std;

#include "HashMapImplementation.h"

// TermDictionaryHashMap
// TermdictionaryValue is just size_t termID
class TermDictionaryHashMap
{
public:
	HashMapImplementation<size_t> m_HashMapHndl;
};

// DocumentTermFreqValue
// DocumentTermFreqValue is just size_t termFreq
class DocumentTermFreqValue
{
public:
	HashMapImplementation<size_t> m_HashMapHndl;
};

#endif