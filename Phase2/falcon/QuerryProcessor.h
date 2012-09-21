#ifndef QUERY_PROCESSOR
#define QUERY_PROCESSOR

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

#include "CommonSettingsAndUtilities.h"
#include "Parameters.h"
#include "IREngine.h"

class BatchQuerryUnit
{
public:
	string m_num;
	string m_title;
	string m_desc;
	string m_narr;
};

class QuerryProcessor
{
	// member fucntions
public:
	QuerryProcessor(Parameters& config);
	~QuerryProcessor();

	void ProcessInBatchMode(string batchFile);
	void ProcessInRealTimeMode();
	vector<BatchQuerryUnit> ParseBatchFile(string batchFile);
	string ExtractFileName(string filePath);
	void AppendResultToFile(string querryNo, vector<DocumentScore>& result, string &resultStr);
	void AddTokensToVector(string str, vector<string> &queryVec);
	void AddTermsToQueryVector(string str, vector<QueryTerm>& vec);
	void TokenizeString(const string& in_target, const string& in_delims, vector< string >& out_vector);
	void DisplayResults(vector<DocumentScore>& results, QueryUnit& queryUnit);
	string GetStaticSnippet(size_t docId);

	void printfiletoauthormap(map<string, string>);
	void dumpAuthorLinks();
	string GetFileName(string);

	// data members
	Parameters m_params;
	IREngine m_IREngine;
};

#endif
