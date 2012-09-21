#ifndef HEAP_DOC_SCORE
#define HEAP_DOC_SCORE

#include <vector>
#include <queue>
#include <iostream>
#include <stack>
using namespace std;

#include "PostingsValue.h"

class DocumentScore
{
public :
	DocumentScore();
	~DocumentScore();

	// operator overloading for priority queue comparison
	//const bool operator< (DocumentScore &val) const;

	int m_docId;
	double m_score;
};

class comparison
{
	bool minheap;
	public:
	comparison(const bool& minparam = true)  //using constructor with default values
	{minheap = minparam;}
	
	bool operator() (const DocumentScore& lhs, const DocumentScore& rhs) const
	{
		if(minheap) return (lhs.m_score > rhs.m_score);   //return true to place lhs over rhs if lhs < rhs i.e. min heap
		else return (lhs.m_score < rhs.m_score);
	}
	
	
};

class HeapDocScore
{
public:
	HeapDocScore();
	~HeapDocScore();

//	priority_queue<DocumentScore> m_sDocScorePriQueue;
	priority_queue<DocumentScore, vector<DocumentScore>, comparison> m_sDocScorePriQueue;

	void ReInitialize(int noOfTopScores);
	void PushDocInHeap(DocumentScore &doc);
	vector<DocumentScore> GetTopDocScores();
	int m_noOfTopScores;
};

#endif
