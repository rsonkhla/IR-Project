#include "HeapDocScore.h"

DocumentScore::DocumentScore()
{
	m_docId	= 0;
	m_score	= 0;
}

DocumentScore::~DocumentScore()
{
}

//const bool DocumentScore::operator< (DocumentScore &val) const
//{
//	return m_score < val.m_score;
//}

HeapDocScore::HeapDocScore()
{
	m_noOfTopScores	= 0;
}

HeapDocScore::~HeapDocScore()
{
}

void HeapDocScore::ReInitialize(int noOfTopScores)
{
	m_noOfTopScores	= noOfTopScores;

	while(m_sDocScorePriQueue.size())
		m_sDocScorePriQueue.pop();
}

vector<DocumentScore> HeapDocScore::GetTopDocScores()
{
	vector<DocumentScore> topDocs;
	stack<DocumentScore> temp;

	while(m_sDocScorePriQueue.size())
	{
		temp.push(m_sDocScorePriQueue.top());
		m_sDocScorePriQueue.pop();
	}

	while(temp.size())
	{
		topDocs.push_back(temp.top());
		temp.pop();
	}

	return topDocs;
}

void HeapDocScore::PushDocInHeap(DocumentScore &doc)
{
	m_sDocScorePriQueue.push(doc);
	if(m_sDocScorePriQueue.size() > m_noOfTopScores)
	{
		m_sDocScorePriQueue.pop();
	}
}
