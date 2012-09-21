#include "IREngine.h"

IREngine::IREngine()
{
}

IREngine::~IREngine()
{
	m_fileNames.close();
	delete m_srchInvIndx;
	delete m_srchAuthInvIndx;
	delete m_srchCatgInvIndx;
}

void IREngine::Initialize(Parameters& config)
{
	// add code
	m_params	= config;

	string dictFolder		= m_params.path_dict_dir;
	string docIdOffsetFile	= dictFolder + "/0_FileDict.txt";
	string docIdNameFile	= dictFolder + "/0_FileNames.txt";

	// generate docId-fileName mapping
	ifstream fpOffset(docIdOffsetFile.c_str());
	if(!fpOffset)
	{
		cout << "Error : can not open docId-fileOffset file." << endl;
	}
	m_fileNames.open(docIdNameFile.c_str());
	if(!m_fileNames)
	{
		cout << "Error : can not open fileName file." << endl;
	}

	string data;
	string line;
	string sDocLen;
	size_t iDocLen;
	string sOffset;
	size_t iOffset;

	string::size_type loc1 = string::npos;
	string::size_type loc2 = string::npos;
	string::size_type loc3 = string::npos;

	while ( !fpOffset.eof() ) 
	{
		getline( fpOffset, line );
		loc1	= line.find(",",0);
		loc2	= line.find(":",loc1+1);
		loc3	= line.find(":",loc2+1);
		sDocLen.clear();
		sDocLen.append(line, loc1+1, loc2-loc1);
		iDocLen	= atoi(sDocLen.c_str());
		sOffset.clear();
		sOffset.append(line, loc3+1, line.size()-loc2);
		iOffset	= atoi(sOffset.c_str());

		m_fileNameOffset.push_back(make_pair(iDocLen, iOffset));
	}

	fpOffset.close();

	// get corpus stats
	ifstream fp;
	fp.open((m_params.path_inv_index_dir + "/CorpusStats.txt").c_str());
	if(!fp)
	{
		cout << "Error : Can not open CorpusStats.txt." << endl;
	}

	string avgDocSz;
	getline( fp, line );
	loc1	= line.find(":",0);
	avgDocSz.append(line, loc1+1, line.size()-loc1);
	m_AvgDocSize	= atoi(avgDocSz.c_str());

	string noOfDocs;
	getline( fp, line );
	loc2	= line.find(":",0);
	noOfDocs.append(line, loc2+1, line.size()-loc2);
	m_NoOfDocs	= atoi(noOfDocs.c_str());

	// serach index pointer
	m_srchInvIndx		= new SearchIndex(config.path_inv_index_dir.c_str(), config.inverted_index_barrels);
	m_srchAuthInvIndx	= new SearchIndex(config.path_auth_inv_index_dir.c_str(), 1);
	m_srchCatgInvIndx	= new SearchIndex(config.path_catg_inv_index_dir.c_str(), 1);
}

bool IREngine::IsThisAWikiDocument(string filePath)
{
	string::size_type loc	= string::npos;
	string sSrchStr			= "/wiki";

	loc	= filePath.find(sSrchStr, 0);
	if(loc != string::npos)
	{
		return true;
	}

	return false;
}

string IREngine::GetFileNameUsingDocId(size_t docId)
{

	size_t offSet	= m_fileNameOffset[docId].second;
	string fileName;


	m_fileNames.seekg(offSet);
	char buffer[200];
	
	m_fileNames.get(buffer, 200, '$');

	string rtrn(buffer);

	return buffer;
}

size_t IREngine::GetDocLenUsingDocId(size_t docId)
{

	size_t docLen	= m_fileNameOffset[docId].first;

	return docLen;
}

vector<DocumentScore> IREngine::ProcessQuery(QueryUnit queryunit)
{
	vector<DocumentScore> result;
	m_heap.ReInitialize(atoi( queryunit.m_numOfResults.c_str() ));
	vector<string> queryVec	= GetQueryVectorString(queryunit);
	vector< list<PostingValue> >  vecOfLstOfTermPostings;
	if(queryVec.size())
		GetVecOFPostinsList(*m_srchInvIndx, queryVec, vecOfLstOfTermPostings);

	vector<string> &authVec		= queryunit.m_author;
	bool IsAuthorInputPresent	= authVec.size();
	vector< list<PostingValue> >  vecOfLstOfAuthPostings;
	if(authVec.size())
		GetVecOFPostinsList(*m_srchAuthInvIndx, authVec, vecOfLstOfAuthPostings);

	vector<string> &catgVec		= queryunit.m_category;
	bool IsCatgInputPresent		= catgVec.size();
	vector< list<PostingValue> >  vecOfLstOfCatgPostings;
	if(catgVec.size())
		GetVecOFPostinsList(*m_srchCatgInvIndx, catgVec, vecOfLstOfCatgPostings);

	vector<size_t> termDocFreq;
	int size	= vecOfLstOfTermPostings.size();
	for(int i=0 ; i<size ; i++)
	{
		termDocFreq.push_back(vecOfLstOfTermPostings[i].size());
	}

	size_t authDocId	= 0;
	size_t catgDocId	= 0;

	while( !PostingListIsProcessed(vecOfLstOfTermPostings) )
	{
		size_t docId					= 0;
		vector<PostingValue> docVector	= GetNextDocumentVector(vecOfLstOfTermPostings, docId);

		if(IsAuthorInputPresent)
		{
			while(authDocId < docId)
			{
				if(PostingListIsProcessed(vecOfLstOfAuthPostings))
					break;
				vector<PostingValue> authVector	= GetNextDocumentVector(vecOfLstOfAuthPostings, authDocId);
				//break;
			}

			if(authDocId != docId)
				continue;
		}

		if(IsCatgInputPresent)
		{
			while(catgDocId < docId)
			{
				if(PostingListIsProcessed(vecOfLstOfCatgPostings))
					break;

				vector<PostingValue> authVector	= GetNextDocumentVector(vecOfLstOfCatgPostings, catgDocId);
				//break;
			}

			if(catgDocId != docId)
					continue;
		}

		double score					= GetSimilarityScore(docVector, queryunit.m_queryVector, termDocFreq);

		if(docId)
		{
			DocumentScore docScore;
			docScore.m_docId	= docId;
			docScore.m_score	= score;

			m_heap.PushDocInHeap(docScore);
		}
	}
	result	= m_heap.GetTopDocScores();
	return result;
}

void IREngine::GetVecOFPostinsList(SearchIndex &srchHndl, vector<string> queryVec, vector< list<PostingValue> > &vecPostings)
{
	int size	= queryVec.size();

	for(int i=0 ; i<size ; i++)
	{
		list<PostingValue> psting;
		srchHndl.GetPostingList(queryVec[i], psting);
		vecPostings.push_back(psting);
	}
}

vector<string> IREngine::GetQueryVectorString(QueryUnit &queryunit)
{
	vector<string> strVec;

	int size	= queryunit.m_queryVector.size();

	for(int i=0 ; i<size ; i++)
	{
		strVec.push_back(queryunit.m_queryVector[i].m_term);
	}

	return strVec;
}

bool IREngine::PostingListIsProcessed(vector< list<PostingValue> > & vecOfLstOfTermPostings)
{
	int size	= vecOfLstOfTermPostings.size();

	for(int i=0 ; i<size ; i++)
	{
		if(vecOfLstOfTermPostings[i].size() > 0 )
		{
			return false;
		}
	}

	return true;
}

vector<PostingValue> IREngine::GetNextDocumentVector(vector< list<PostingValue> > & vecOfLstOfTermPostings, size_t &docId)
{
	vector<PostingValue>	docVec;
	docId	= 0;

	size_t size		= vecOfLstOfTermPostings.size();
	size_t minDocId	= UINT_MAX;

	for(size_t i=0 ; i<size ; i++)
	{
		if(vecOfLstOfTermPostings[i].size())
		{
			if(minDocId	> (*(vecOfLstOfTermPostings[i].begin())).m_iDocId )
			{
				minDocId	= (*(vecOfLstOfTermPostings[i].begin())).m_iDocId;
			}
		}
	}

	for(size_t i=0 ; i<size ; i++)
	{
		PostingValue val;
		val.m_iDocId	= 0;
		val.m_iTermFreq	= 0;

		if(vecOfLstOfTermPostings[i].size())
		{
			if(minDocId	== (*(vecOfLstOfTermPostings[i].begin())).m_iDocId )
			{
				val		= (*(vecOfLstOfTermPostings[i].begin()));
				docId	= val.m_iDocId;
				vecOfLstOfTermPostings[i].erase(vecOfLstOfTermPostings[i].begin());
			}
		}
		docVec.push_back(val);
	}

	return docVec;
}

double IREngine::GetSimilarityScore(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq)
{
	if(m_params.m_mode	== "--boolean")
	{
		return BooleanSimilarity(docVector, queryVector, termDocFreq);
	}
	else if(m_params.m_mode	== "--cosine")
	{
		return CosineSimilarity(docVector, queryVector, termDocFreq);
	}
	else
	{
		return OkapiSimilarity(docVector, queryVector, termDocFreq);
	}
}

double IREngine::BooleanSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq)
{
	double score	= 0;
	int size		= queryVector.size();

	for(int i=0 ; i<size ; i++)
	{
		score += (docVector[i].m_iTermFreq > 0)*(queryVector[i].m_freq > 0);
	}

	return score;
}

double IREngine::CosineSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq)
{
	double score	= 0;
	int size		= queryVector.size();
	double qtf		= 0;
	double wqtf		= 0;
	double dtf		= 0;
	double wdtf		= 0;

	double lenQuery	= 0;
	double lenDoc	= 0;

	for(int i=0 ; i<size ; i++)
	{
		qtf			= queryVector[i].m_freq;
		dtf			= docVector[i].m_iTermFreq;

		wqtf		= (1 + log(qtf));

		if(dtf)
			wdtf	= (1 + log(dtf)) * log((double)m_NoOfDocs / termDocFreq[i]);
		else
			wdtf	= 0;

		lenQuery	+= pow(qtf, 2.0);
		lenDoc		+= pow(dtf, 2.0);

		score += (wqtf)*(wdtf);
	}

	// normalize
	if(lenDoc)
		score	/= (lenQuery*lenDoc);
	else
		score	= 0;

	return score;
}

double IREngine::OkapiSimilarity(vector<PostingValue> &docVector, vector<QueryTerm> &queryVector, vector<size_t> &termDocFreq)
{
	double score	= 0;
	int size		= queryVector.size();

	double qtf		= 0;
	double dtf		= 0;
	double docLen	= 0;

	double k1		= 1.2;
	double k3		= 1.4;
	double b		= 0.75;

	for(int i=0 ; i<size ; i++)
	{
		qtf			= queryVector[i].m_freq;
		dtf			= docVector[i].m_iTermFreq;
		docLen		= GetDocLenUsingDocId(docVector[i].m_iDocId);

		if(termDocFreq[i])
		{
			score	+=  ((log((double)m_NoOfDocs / termDocFreq[i])) * ((k1+1)*dtf) * ((k3+1)*qtf)) /
						((k1*((1-b) + b*(docLen/m_AvgDocSize) + dtf))*(k3+qtf));
		}
	}

	return score;
}
