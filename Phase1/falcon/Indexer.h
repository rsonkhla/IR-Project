/*
Author			: Shishir Garg
UB Person No	: 50026818
*/
#ifndef INDEXER
#define INDEXER

#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include "Parameters.h"

using namespace std;

class index_class
{
	map<string, set<size_t> > index;
	size_t postings_count;
	static int split_no;
	int index_barrels;
	string index_temp_path;
	string index_path;
public:
	index_class(parameters p);
	void update_index(vector<string>&, size_t);
	void write_to_file();
	void MergeIndex();
private:
	//void printList(list<int>& l);
	void mergeIndices(map<string, list<int> >& index1, map<string, list<int> >& index2);
	void merge(string infile1, string infile2, string outfile);
	//void printIndex(map<string, list<int> >& index);
	void dumpIndexToCSV(map<string, list<int> >& index, ofstream& ofile);
	void readCSVIndexIntoMap(ifstream& data, map<string, list<int> >& index);
}; //class ends

#endif
