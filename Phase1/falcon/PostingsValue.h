#ifndef POSTINGS_VALUE
#define POSTINGS_VALUE

#include <stdlib.h>

using namespace std;

// PostingValue
class PostingValue
{
	// member fucntions
public:
	PostingValue();
	~PostingValue();

	// operator overloading for priority queue comparison
	const bool operator < (PostingValue &pstngVal) const;

	// data members
public:
	size_t m_iDocId;
	size_t m_iTermFreq;
};

#endif
