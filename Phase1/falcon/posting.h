#ifndef POSTING_H_
#define POSTING_H_
#include <sstream>
#include <list>
#include <string>
#include <utility>
#include <cstdlib>
#include <iostream>

using namespace std;



class Posting
{
	public:
		string term;
		string size;
		list<pair<size_t, size_t> > postingList;
		string fileFrom;
		string postingListAsString;


		void mergePostingList(Posting& p);

		Posting(const char* lineS, const char* fileFrom);



		Posting();

	private:
		string getPostListAsString(list<pair<size_t, size_t> > lst);
		void parseString(const char* lineString);

};

class PostingComparator
{
	bool minheap;
	public:
		PostingComparator(const bool& m);
		PostingComparator();
		bool operator()(Posting& p1, Posting& p2);
};

template< typename FirstType, typename SecondType >
struct DocIdFreqComparator
{
		bool operator()(const std::pair<FirstType, SecondType>& p1, const std::pair<FirstType, SecondType>& p2);
};


#endif
