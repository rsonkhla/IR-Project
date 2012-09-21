/*
 * searchindex.h
 *
 *  Created on: Oct 29, 2011
 *      Author: ashu1710
 */

#ifndef SEARCHINDEX_H_
#define SEARCHINDEX_H_
#include <cctype>
#include <list>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <iostream>

#include "HashMapImplementation.h"
#include "PostingsValue.h"


class SearchIndex
{
	public:
		SearchIndex(const char* indexDirectory, int x);
		SearchIndex();
		void GetPostingList(string term, list<PostingValue>&);

	private:
		void parsePostingList(const char* postingstr, list<PostingValue>& lst);
		string indexDirectory;
		int partitions;
		HashMapImplementation<pair<int, int> > indexOffMap;
		void readOffsetMapToHash();
		int getBarrelNumber(char c);
};
#endif
