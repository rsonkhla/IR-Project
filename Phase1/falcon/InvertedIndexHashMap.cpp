#include "InvertedIndexHashMap.h"

InvertedIndexValue::InvertedIndexValue()
{
	m_iDocFreq	= 0;
	m_iPstngLen	= 0;
}

InvertedIndexValue::~InvertedIndexValue()
{
}

InvertedIndexHashMap::InvertedIndexHashMap()
{
}

InvertedIndexHashMap::InvertedIndexHashMap(string outputDirectory, size_t barrelNo, size_t tokensLimitPerBarrel)
{
	m_sOutputPath			= outputDirectory;
	m_iBarrelNo				= barrelNo;
	m_iPartitionNo			= 0;
	m_tokensLimitPerBarrel	= tokensLimitPerBarrel;
	m_noOfTokensProcessed	= 0;
}

InvertedIndexHashMap::~InvertedIndexHashMap()
{
}

void InvertedIndexHashMap::SetDataMembers(string outputDirectory, size_t noOfBarrels, size_t tokensLimitPerBarrel)
{
	m_sOutputPath			= outputDirectory;
	m_iBarrelNo				= noOfBarrels;
	m_iPartitionNo			= 0;
	m_tokensLimitPerBarrel	= tokensLimitPerBarrel;
	m_noOfTokensProcessed	= 0;
}

void InvertedIndexHashMap::AddData(size_t docId, vector<string> toks)
{
	// clear old doc term freq distribution
	m_docTermFreq.internalMap.clear();
	int defaultCount	= 1;

	// generate term freq for this doc
	for(vector<string>::iterator it = toks.begin(); it != toks.end(); it++)
	{
		//cout << *it << endl;
		// get that term
		if(!m_docTermFreq.IsKeyPresent((*it).c_str()))
		{
			//m_docTermFreq.InsertKeyValuePair((*it).c_str(), int());
			m_docTermFreq.InsertKeyValuePair((*it).c_str(), defaultCount);
		}
		int &freq	= m_docTermFreq.GetValue((*it).c_str());
		// update it
		freq++;		
	}

	// foreach term in m_docTermFreq
	string			term;
	int				freq;
	stringstream	pstngTmp;
	string			pstng;
	int				len;

	//InvertedIndexValue	tmpIndxVal;

#ifdef MY_WINDOWS
	for(stdext::hash_map<std::string, int>::iterator it = m_docTermFreq.internalMap.begin() ; it != m_docTermFreq.internalMap.end() ; it++)
#else
	//for(hash_map<const char*, int, hash<const char*>, eqstr >::iterator it = m_docTermFreq.internalMap.begin() ; it != m_docTermFreq.internalMap.end() ; it++)
	for(map<string, int >::iterator it = m_docTermFreq.internalMap.begin() ; it != m_docTermFreq.internalMap.end() ; it++)
#endif
	{
		// get that term
		term	= (*it).first;
		freq	= (*it).second;

		//cout << "Term : " << term << endl << "Freq : " << freq << endl << endl;

		if(!m_invIndx.IsKeyPresent(term.c_str()))
		{
			m_invIndx.InsertKeyValuePair(term.c_str(), InvertedIndexValue());
			//m_invIndx.InsertKeyValuePair(term.c_str(), tmpIndxVal);
		}
		InvertedIndexValue &val	= m_invIndx.GetValue(term.c_str());
		
		// generate its pstng
		pstngTmp.str("");
		pstngTmp << docId << " " << freq << ",";
		pstng	= pstngTmp.str();
		len		= pstng.size();
		
		// update it 
		val.m_iDocFreq++;
		val.m_iPstngLen	+= len;
		val.m_sPstngLst.append(pstng);

		//cout << term << " " << val.m_iDocFreq << " " << val.m_iPstngLen	<< " " << val.m_sPstngLst << endl;
	}

	// dump logic
	m_noOfTokensProcessed	+= toks.size();
	if(m_noOfTokensProcessed >= m_tokensLimitPerBarrel)
	{
		m_noOfTokensProcessed	= 0;
		DumpInvertedIndex();
	}
}

void InvertedIndexHashMap::DumpInvertedIndex()
{
	stringstream fp;
	fp << m_iBarrelNo;
	fp << ".";
	fp << m_iPartitionNo++;

	string tempStr;
	tempStr	= m_sOutputPath;
	tempStr.append(fp.str());

	ofstream indxDump;
	indxDump.open(tempStr.c_str());
	if (!indxDump) 
	{
		cout << "Error : Can not open file for dumping file index." << endl;
	}

	//cout << m_invIndx.GetValue("york").m_iPstngLen << " " << m_invIndx.GetValue("york").m_sPstngLst << endl;
	// debug
	//cout << "size : ";
	//cout << m_invIndx.internalMap.size() << endl;
	int count1	= 0;

	// sort terms
	vector<string> forSorting;
	int count	= 0;
#ifdef MY_WINDOWS
	for(stdext::hash_map<std::string, InvertedIndexValue>::iterator it = m_invIndx.internalMap.begin() ; it != m_invIndx.internalMap.end() ; it++)
#else
	//for(hash_map<const char*, InvertedIndexValue, hash<const char*>, eqstr >::iterator it = m_invIndx.internalMap.begin() ; it != m_invIndx.internalMap.end() ; it++)
	for(map<string, InvertedIndexValue >::iterator it = m_invIndx.internalMap.begin() ; it != m_invIndx.internalMap.end() ; it++)
#endif
	{
		//cout << ++count1 << (*it).first << endl;
		forSorting.push_back(string());
		forSorting[count++]	= (*it).first;
	}
	sort(forSorting.begin(), forSorting.end());

	// dump postings
	string			term;
	stringstream	pstngTmp;
	for(vector<string>::iterator it = forSorting.begin() ; it != forSorting.end() ; it++)
	{
		term	= *it;
		InvertedIndexValue &val	= m_invIndx.GetValue(term.c_str());
		
		// generate posting string to be dumped
		pstngTmp.str("");
		pstngTmp << term << ":" << val.m_iPstngLen << ":" << val.m_sPstngLst << "\n";
		indxDump << pstngTmp.str();
	}

	indxDump.close();	

	// clear hash_map
	m_invIndx.internalMap.clear();
}
