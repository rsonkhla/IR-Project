#include "posting.h"


using namespace std;

template< typename FirstType, typename SecondType >
bool DocIdFreqComparator<FirstType, SecondType>::operator()(const std::pair<FirstType, SecondType>& p1, const std::pair<FirstType, SecondType>& p2)
{
  return (p1.first < p2.first);
}


bool PostingComparator::operator()(Posting& p1, Posting& p2)
{
	if(minheap)
		return (p1.term < p2.term);
	else
		return (p1.term > p2.term);
}

PostingComparator::PostingComparator(const bool& minparam)
{
	this->minheap = minparam;
}

PostingComparator::PostingComparator()
{

}
string Posting::getPostListAsString(list<pair<size_t, size_t> > lst)
{
	stringstream str;
	for(list<pair<size_t, size_t> >::iterator it = lst.begin(); it != lst.end(); it++)
	{
		str << it->first << "," << it->second << " ";
	}
	string resultString = str.str();
	//resultString.erase(resultString.end() - 1);
	resultString.push_back('\n');
	return resultString;
}

void Posting::mergePostingList(Posting& other)
{
	DocIdFreqComparator<size_t, size_t> comp;
	//cout << "---------------------"<<endl;
	//cout << "terms: " << other.term << " & " << this->term << endl;
	//cout << "lists : ";

	//getPostListAsString(other.postingList);
	//cout << other.postingListAsString << "||" << this->postingListAsString << endl;
	//getPostListAsString(this->postingList);

	postingList.merge(other.postingList, comp);
	this->postingListAsString = getPostListAsString(this->postingList);
	//cout << "MergedList: " << this->postingListAsString << endl;
	//getPostListAsString(this->postingList);
	//cout << endl;
}


void Posting::parseString(const char* lineString)
{
	//cout << "ParseString: LineString: "<< lineString << endl;
	int i;


	//get term
	for(i = 0; (lineString[i] != ':') && (lineString[i] != '\0') && (lineString[i] != '\n'); i++)
	{

		term.push_back(lineString[i]);
	}
	i++;

	//skip over length
	for(i = i; lineString[i] != ':' && (lineString[i] != '\0') && (lineString[i] != '\n'); i++)
		size.push_back(lineString[i]);
	i++;

	//get pairs
	string curDocId;
	string curFreq;

	bool nowDocId = true;
	for(i = i; (lineString[i] != '\0'); i++)
	{
		//postingListAsString.push_back(lineString[i]);
		if(lineString[i] == ',')
		{
			nowDocId = true;
			pair<size_t, size_t> newPair;

			newPair.first = atoi(curDocId.c_str());
			newPair.second = atoi(curFreq.c_str());

			postingList.push_back(newPair);

			curDocId.clear();
			curFreq.clear();
		}
		else if(lineString[i] == ' ')
			nowDocId = false;
		else if(nowDocId)
			curDocId.push_back(lineString[i]);
		else
			curFreq.push_back(lineString[i]);

	}

}


Posting::Posting(const char* lineS, const char* fileFrom)
{
	parseString(lineS);
	this->fileFrom += fileFrom;
	this->postingListAsString = getPostListAsString(this->postingList);
}

Posting::Posting()
{
	//parseString(lineS);
}


