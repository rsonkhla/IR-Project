#ifndef IR_ENGINE
#define IR_ENGINE

#include "Parameters.h"
#include "searchindex.h"
#include "HeapDocScore.h"
#include <math.h>
#include <string>

class QueryTerm
{
public:
	string	m_term;
	size_t	m_freq;
};

class QueryUnit
{
public:
	string m_numOfResults;
	vector<QueryTerm> m_queryVector;
	vector<string> m_author;
	vector<string> m_category;
};

class IREngine
{
public:
	IREngine();
	~IREngine();

	void Initialize(Parameters& config);
	string GetFileNameUsingDocId(size_t docId);
	bool IsThisAWikiDocument(string filePath);
	size_t GetDocLenUsingDocId(size_t docId);
	vector<DocumentScore> ProcessQuery(QueryUnit queryunit);

	vector<string> GetQueryVectorString(QueryUnit &queryunit);
	void GetVecOFPostinsList(SearchIndex &srchHndl, vector<string> queryVec, vector< list<PostingValue> > &vecPostings);
	bool PostingListIsProcessed(vector< list<PostingValue> > & vecOfLstOfTermPostings);
	vector<PostingValue> GetNextDocumentVector(vector< list<PostingValue> > & vecOfLstOfTermPostings, size_t &docId);
	double GetSimilarityScore(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq);
	double BooleanSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq);
	double CosineSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq);
	double OkapiSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq);

	Parameters m_params;
	ifstream m_fileNames;
	vector< pair<size_t, size_t> > m_fileNameOffset; 
	size_t m_AvgDocSize;
	size_t m_NoOfDocs;

	SearchIndex *m_srchInvIndx;
	SearchIndex *m_srchAuthInvIndx;
	SearchIndex *m_srchCatgInvIndx;

	HeapDocScore	m_heap;
};

#endif
