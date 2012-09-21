/*Author: Shishir Garg*/

#include "Indexer.h"
#include "CommonSettingsAndUtilities.h"
#include <stdlib.h>

int index_class::split_no = 0;
//string index_class::index_temp_path = "../index_temp_files";
//size_t index_class::postings_count;

index_class::index_class(parameters p)
{
	this->index_barrels = p.inverted_index_barrels;
	this->index_path = p.path_index_dir;
	this->index_temp_path = "./index_temp_files";
	//CreateNewDirectory(index_temp_path);
	//CreateNewDirectory(index_path);
}

void index_class::update_index(vector<string>& terms, size_t docID)
{
	for(vector<string>::iterator i = terms.begin(); i != terms.end(); i++)
	{
		index[*i].insert(docID);
	}
	
	//if((terms.size() + postings_count) > 50000) //postings added to index per doc = number of terms in document
	if(postings_count + terms.size() > 50000)//or index.size > 50000
	{
		//cout << "Calling WriteToFile" << endl;
		write_to_file();
		postings_count = 0;
		split_no++;
		index.clear();
	}
		else
	{
		postings_count += terms.size();
	}
} // update index ends

void index_class::write_to_file()
{	
	//cout <<"Enter number of files\n";		
	//int no_of_files = 3; //or read from variable.
	
	//declare vector of ofstreams
	vector<ofstream*> ofile_vect;
	stringstream ss;
	
	//assign ofstreams to files
	for(int i= 0; i<index_barrels; i++)
	{
		
		ss << index_temp_path + "/index.inv." << setfill('0') << setw(3) << i << "." << setw(4) << split_no;
		//cout << "creating ofstreams : " << ss.str() << endl;
		ofile_vect.push_back(new ofstream((ss.str()).c_str()));
		//ofile_vect[i] = new ofstream(ss.str().c_str());
		ss.str("");
	}
		map<string, set<size_t> >::iterator it;
	
	int m,n,bin;
		//start iterating over map elements and write to file
	for(it = index.begin(); it != index.end(); it++)
	{
		m = (int)((*it).first.at(0));
		
		if(((m <= '9') && (m >= '0')) || (m == '-') )
		{
			m = 'a';
			n = 'a';
		}

		if((*it).first.size() > 1)
		{
			n = (int)((*it).first.at(1));
			if(n == '-')
				n = 'a';
		}
		else
			n = 'a';

		
		//cout << m << " " << n << endl;
		//cout << "term is " << (*it).first << endl;
		bin = (int)((double)((m-97)*26 + (n-97)) / (double)676 * index_barrels);
		
		//cout << "writing term to file " << bin << endl;
		*(ofile_vect[bin]) << (*it).first;
		//cout <<"wrote term\n";
		set<size_t>::iterator itdid;
		
		//write postings list
		for(itdid = ((*it).second).begin(); itdid != ((*it).second).end(); itdid++)
		{
			*(ofile_vect[bin]) << "," << *itdid;
		}
		
		(*(ofile_vect[bin])).flush();
		*(ofile_vect[bin]) << "\n";		
		(*it).second.clear();
	}
		//close all ofstreams
		for(int i = 0; i< ofile_vect.size(); i++)
		{
			(*(ofile_vect[i])).close();
			delete ofile_vect[i];
		}
}  //write_to_file ends finally


void index_class::MergeIndex()
{
	stringstream ssi, ssj;

	//cout << "Merging files" << endl;
	map<string, list<int> > index;
	for(int i = 0; i<this->index_barrels; i++)  //loops through different alphabet bins
	{
		ssi << this->index_path << "/index.inv." << setfill('0') << setw(3) << i;
		//cout << ssi.str() << endl;
		index.clear();
		for(int j=0; j<=(this->split_no+1); j++) //loops through different splits in same alphabet bracket.
		{
			ssj << this->index_temp_path + "/index.inv." << setfill('0') << setw(3) << i << "." << setw(4) << j;
			map<string, list<int> > temp;
			ifstream curmap(ssj.str().c_str());
			readCSVIndexIntoMap(curmap, temp);
			mergeIndices(index, temp);
			temp.clear();
			unlink(ssj.str().c_str());   //delete ssj file after merge 		
			ssj.str("");
			curmap.close();
		}
		
		//ifstream finali(ssi.str().c_str());
		//readCSVIndexIntoMap(finali, map);
		int x;
		ofstream postings((ssi.str() + ".pstngs").c_str());
		ofstream indexf(ssi.str().c_str());
		ssi.str("");
		int offset = 0;
		string postingJoins;
		for(map<string, list<int> >::iterator it = index.begin(); it != index.end(); it++)
		{
			stringstream postingjoin("");
			
			for(list<int>::iterator jt = (it->second).begin(); jt != (it->second).end(); jt++)
			{
				if(jt == (it->second).begin())
					postingjoin << (*jt);
				
				else
					postingjoin << "," << *jt;	
			}
			//cout << postingjoin.str();
			//cin >> x;
			postingJoins += (postingjoin.str() + "$");
			indexf << it->first << "," << offset << endl;
			offset += (postingjoin.str().size() + 1);
			postingjoin.str("");
		}
		postings << postingJoins;
		postings.close();
		indexf.close();		
	}
	
}

void index_class::dumpIndexToCSV(map<string, list<int> >& index, ofstream& ofile)
{
	for(map<string, list<int> >::iterator it = index.begin(); it != index.end(); it++)
	{
		ofile << it->first;
		for(list<int>::iterator jt = (it->second).begin(); jt != (it->second).end(); jt++)
			ofile << ","<<*jt;
		ofile << endl;
	}
}

void index_class::readCSVIndexIntoMap(ifstream& data, map<string, list<int> >& index)
{
	string line;
	int cellcount;
	while(getline(data, line))
	{
		stringstream stream(line);
		cellcount = 0;
		string cell;
		string currentToken;

		while(getline(stream, cell, ','))
		{
			//cout << cell << endl;
			if(cellcount == 0)
			{
					list<int> postings;
					index[cell] = postings;
					currentToken = cell;
			}
			else
				index[currentToken].push_back(atoi(cell.c_str()));
			cell = "";
			cellcount++;
		}
	}

}

//void index_class::printList(list<int>& l)
//{
//	for(list<int>::iterator jt = l.begin(); jt != l.end(); jt++)
//				cout << *jt << " ";
//}

void index_class::mergeIndices(map<string, list<int> >& index1, map<string, list<int> >& index2)
{
	for(map<string, list<int> >::iterator it = index2.begin(); it != index2.end(); it++)
	{
		if(index1.find(it->first) == index1.end())
			index1[it->first] = it->second;
		else
		{
			index1[it->first].merge(it->second);
		}
	}
}

void index_class::merge(string infile1, string infile2, string outfile)
{	
	//ofstream outf(outfile.c_str());
	ifstream inf1(infile1.c_str()), inf2(infile2.c_str());

	map<string, list<int> > index1, index2;

	readCSVIndexIntoMap(inf1, index1);
	readCSVIndexIntoMap(inf2, index2);

	mergeIndices(index1, index2);

	index2.clear();
	inf1.close();
	inf2.close();

	ofstream outf(outfile.c_str(), ios_base::trunc);
	dumpIndexToCSV(index1, outf);

	index1.clear();
	outf.close();
	
}


//void index_class::printIndex(map<string, list<int> >& index)
//{
//	for(map<string, list<int> >::iterator it = index.begin(); it != index.end(); it++)
//	{
//		cout << it->first <<": ";
//		for(list<int>::iterator jt = (it->second).begin(); jt != (it->second).end(); jt++)
//			cout << *jt << " ";
//		cout << endl;
//	}
//}
