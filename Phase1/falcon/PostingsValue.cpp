#include "PostingsValue.h"


PostingValue::PostingValue()
{
	m_iDocId	= 0;
	m_iTermFreq	= 0;
}

PostingValue::~PostingValue()
{
}

const bool PostingValue::operator< (PostingValue &pstngVal) const
{
	return m_iTermFreq > pstngVal.m_iTermFreq;
}