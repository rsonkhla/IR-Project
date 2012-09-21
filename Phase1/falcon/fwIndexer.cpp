// Author: Shishir Garg

#include "fwIndexer.h"

fwIndexer::fwIndexer()
{
	section_map["<#ARTICLE_NAME>"] = 0;
	section_map["<#AUTHOR>"] = 1;
	section_map["<#TIMESTAMP>"] = 2;
	section_map["<#INFOBOX>"] = 3;
	section_map["<#TAXOBOX>"] = 4;
	section_map["<#SECTIONS>"] = 5;
	section_map["<#LINKS>"] = 6;
	section_map["<#CATEGORY>"] = 7;
	doc_count = 0;
	//index_file_path = "../index_files";
	//CreateNewDirectory((index_file_path + "/fwdIndex"));
}

fwIndexer::fwIndexer(string p)
{
	section_map["<#ARTICLE_NAME>"] = 0;
	section_map["<#AUTHOR>"] = 1;
	section_map["<#TIMESTAMP>"] = 2;
	section_map["<#INFOBOX>"] = 3;
	section_map["<#TAXOBOX>"] = 4;
	section_map["<#SECTIONS>"] = 5;
	section_map["<#LINKS>"] = 6;
	section_map["<#CATEGORY>"] = 7;
	doc_count = 0;
	index_file_path = p;
}
void fwIndexer::makeIndex(size_t docID, string& docData, map<string, size_t>& termDict)
{
	fwIndexer::docID = docID;
	(fwIndex[docID]).first.resize(8);

	doc_count++;
	//istringstream iss;
	stringstream buffer(docData);
	//cout << buffer.str();
	int sectionID = 0, termID = 0;
	
	while(buffer.tellg() >= 0)
	{
		string sub;
		buffer >> sub;
		//cout << buffer.str();
		//cout << sub << " " << buffer.tellg() << " ";
		if(section_map.find(sub) != section_map.end())
		{
			sectionID = section_map[sub];   //change sectionID
			//cout  << sectionID;
		}

		else
		{
			termID = termDict[sub];   	//find term ID
		   	fwIndex[docID].second.push_back(termID);     //write termID to vector
			(fwIndex[docID].first[sectionID])++;	  //update term count of section
			
		}
	} //while ends

	buffer.str("");

	//writing to file part

	if(doc_count%10000 == 0)
	{
		this->writeIndex();
		fwIndex.clear();//clear map
		
	}
}  //read index ends

void fwIndexer::writeIndex()
{
	stringstream ss;
	ss << index_file_path << "/" << "index.fwd." << setfill('0') << setw(3) << (int)(doc_count/10000);
	ofstream ofile((ss.str()).c_str());
	//write all docIDs at top of file
	for(map <size_t, pair<vector<int>, vector<size_t> > >::iterator mit = fwIndex.begin(); mit != fwIndex.end(); mit++)
	{
	   ofile << (*mit).first << " ";
	}
	
	ofile << "\n";
	
	for(map <size_t, pair<vector<int>, vector<size_t> > >::iterator mit = fwIndex.begin(); mit != fwIndex.end(); mit++)
	{
	   int cumul_term_count = 0;
	   ofile << (*mit).first << " ";  //write doc ID
	   vector<size_t>::iterator vit = (*mit).second.second.begin();
	   for(int i=0; i < 8; i++)
	   {
		cumul_term_count += fwIndex[docID].first[i];    //update cumulative term count
		ofile << fwIndex[docID].first[i] << " ";   //write term count for section i
	   	for(; (vit - (*mit).second.second.begin()) < cumul_term_count; vit++)
	      	{
		    //cout << *vit << endl;
		    ofile << *vit << " ";
	        }
	   }
	   ofile << "\n";
	}
	ofile.close();
}//writeIndex ends
