/*
 * Reader.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashu1710
 */

#include "reader.h"

using namespace std;

IndexReader::IndexReader()
{
	// TODO Auto-generated constructor stub


}

Posting IndexReader::readLinesFromFile(ifstream& ifile, string filename)
{

		string buf;
		getline(ifile, buf);
		//cout << "Getline: " << buf << endl;
		Posting newPost(buf.c_str(), filename.c_str());
		return newPost;
}
