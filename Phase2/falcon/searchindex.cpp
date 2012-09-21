/*
 * searchindex.cpp
 *
 *  Created on: Oct 29, 2011
 *      Author: ashu1710
 */

#include "searchindex.h"

SearchIndex::SearchIndex()
{
	// TODO Auto-generated constructor stub

}

SearchIndex::SearchIndex(const char* fname, int x)
{
	this->indexDirectory += fname;
	this->partitions = x;
	this->readOffsetMapToHash();
	// TODO Auto-generated constructor stub
}

void SearchIndex::parsePostingList(const char* postingstr, list<PostingValue>& lst)
{

	string curDocId;
	string curTermId;
	bool nowDocId = true;
	for(size_t i = 0; (postingstr[i] != '\0') && (postingstr[i] != '\n'); i++)
	{

		if(postingstr[i] == ' ')
		{
			PostingValue pv;
			pv.m_iDocId = atoi(curDocId.c_str());
			pv.m_iTermFreq = atoi(curTermId.c_str());
			lst.push_back(pv);
			nowDocId = true;
			curDocId.clear();
			curTermId.clear();
		}
		else if(postingstr[i] == ',')
		{
			nowDocId = false;
		}
		else
			if(nowDocId)
				curDocId.push_back(postingstr[i]);
			else
				curTermId.push_back(postingstr[i]);

	}
}


void SearchIndex::readOffsetMapToHash()
{
       //cout << "Readin Index!!"<<endl;
       char filename[50];
       string line;
       string term;
       pair<int, int> curpair;
       int offset;
       int size;
       string temp;
       for(int i = 0; i < this->partitions; i++)
       {
               sprintf(filename, "%s/index.inv.%d", this->indexDirectory.c_str(), i);
               //cout<<  filename << endl;
               ifstream ifile(filename);
               ifile.seekg(0, ios_base::end);
               int length = ifile.tellg();
               ifile.seekg(ios_base::beg);

               if(length <= 2)
               {
                       cout << "Empty file... " << filename << endl;
                       return;
               }
               while(!ifile.eof())
               {
                       getline(ifile, line);
                       ////cout << line << endl;
                       stringstream ss;
                       ss << line;

                       ss >> term;

                       ss >> temp;
                       offset = atoi(temp.c_str());
                       //cout <<"readOffsetMapToHash: size = " << size << endl;

                       ss >> temp;
                       size = atoi(temp.c_str());

                       pair<int, int> curpair;
                       curpair.first = offset;
                       curpair.second = size;

                       this->indexOffMap.InsertKeyValuePair(term.c_str(), curpair);
                       line.clear();
               }

       }

}

int SearchIndex::getBarrelNumber(char c)
{
	if(isalpha(c))

	{
		int letterOffset = c - 'a';
		int barrno = 0;
		int count = ('z' - 'a')/this->partitions;
		for(int i = 0; i < this->partitions; i++)
		{
			if((c >= ('a' + i*count)) && (c <= ('a' + (i + 1)*count)))
				return i;
		}

	}
	else
	{
		return 0;
	}
}

int GetBarrelNo(string str, int noOfBarrels)
{
	int m, n, num;

	if(str.size() == 0)
		return 0;

	m	= str[0];

	if(str.size() > 1)
	{
		n	= str[1];
	}
	else
	{
		n	= 'a';
	}

	if(((m <= '9') && (m >= '0')) || (m == '-') )
	{
		m = 'a';
	}

	if(((n <= '9') && (n >= '0')) || (n == '-') )
	{
		n = 'a';
	}

	num = (int)((double)((m-97)*26 + (n-97)) / (double)676 * noOfBarrels);

	return num;
}

void SearchIndex::GetPostingList(string term, list<PostingValue>& lst)
{
	if(this->indexOffMap.IsKeyPresent(term.c_str()))
	{
		pair<int, int> curpair = this->indexOffMap.GetValue(term.c_str());

		int barno = GetBarrelNo(term, partitions);
		char filename[50];

		sprintf(filename, "%s/index.inv.%d.pstngs", this->indexDirectory.c_str(), barno);

		ifstream pstngfile(filename);
		pstngfile.seekg(curpair.first, ios_base::beg);
		char* rdbuf = new char[curpair.second + 1];
		pstngfile.read(rdbuf, curpair.second);
		rdbuf[curpair.second] = '\0';

		parsePostingList(rdbuf, lst);
		delete [] rdbuf;
		pstngfile.close();
	}
}

