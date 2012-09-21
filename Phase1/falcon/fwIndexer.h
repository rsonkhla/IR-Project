#ifndef FWINDEXER
#define FWINDEXER

#include <iostream>
#include <map>
#include <vector>
#include <fstream>		
#include <sstream>
#include <utility>
#include <iomanip>
#include "Parameters.h"
#include "CommonSettingsAndUtilities.h"

using namespace std;

class fwIndexer
{
	public:
	void makeIndex(size_t, string& , map<string, size_t>&);
	void writeIndex();
	fwIndexer();
	fwIndexer(string p);
	private:
	map < size_t, pair<vector<int>, vector<size_t> > > fwIndex; //map of doc ID to 2 arrays, one for section term frequency, another for term IDs
	size_t docID;
	map<string, int> section_map;
	int doc_count;
	string index_file_path;
		
};

#endif
