#include "QuerryProcessor.h"

QuerryProcessor::QuerryProcessor(Parameters& config)
{
	m_params	= config;

	cout << "Processing in batch mode." << endl << endl;
	cout << "IR engine : Initialization started." << endl;
	m_IREngine.Initialize(m_params);
	cout << "IR engine : Initialization completed." << endl << endl;
}

QuerryProcessor::~QuerryProcessor()
{
}

void QuerryProcessor::ProcessInBatchMode(string batchFile)
{
	vector<BatchQuerryUnit> queries	= ParseBatchFile(batchFile);
	string resultStr;
	string tmpStr;

	for(vector<BatchQuerryUnit>::iterator it = queries.begin() ; it != queries.end() ; it++)
	{
		BatchQuerryUnit &query	= *it;
		int noOfResults			= 20;

		QueryUnit queryunit;
		queryunit.m_numOfResults	= 20;

		if(m_params.m_zone.find("title",0) != string::npos)
		{
			AddTermsToQueryVector(query.m_title, queryunit.m_queryVector);
		}
		else if(m_params.m_zone.find("narr",0) != string::npos)
		{
			AddTermsToQueryVector(query.m_narr, queryunit.m_queryVector);
		}
		else if(m_params.m_zone.find("title+narr",0) != string::npos)
		{
			AddTermsToQueryVector(query.m_desc, queryunit.m_queryVector);
			AddTermsToQueryVector(query.m_narr, queryunit.m_queryVector);
		}

		queryunit.m_numOfResults	= "10";
		vector<DocumentScore> result	= m_IREngine.ProcessQuery(queryunit);

		AppendResultToFile(query.m_num, result, tmpStr);
		resultStr.append(tmpStr);
	}

	cout << resultStr << endl;

	ofstream fp;
	fp.open("./resultOutput.txt");
	if(!fp)
	{
		cout << "Error : Can not open file for dumping relevance results." << endl;
	}
	fp << resultStr;
	fp.close();
}

void ExtractMainName(string &str)
{
	// remove ending -number
	string tmp				= str;
	string::size_type loc1 = string::npos;
	string::size_type loc2 = string::npos;

	loc1	= tmp.find("-", 0);
	if(loc1	!= string::npos)
	{
		str.clear();
		str.append(tmp, 0, loc1);
	}
	
	// replace space with underscore
	tmp	= str;
	int countS	= 0;
	for(int i=0 ; i<str.size() ; i++)
	{
		if( (tmp[i] == ' ' ) || 
			(tmp[i] == '\t') ||
			(tmp[i] == '\n'))
			countS++;
		else
			break;
	}

	int countE	= 0;
	for(int i=str.size()-1 ; i>=0 ; i--)
	{
		if( (tmp[i] == ' ' ) || 
			(tmp[i] == '\t') ||
			(tmp[i] == '\n'))
			countE++;
		else
		{
			break;
		}
	}

	str.clear();
	str.append(tmp, countS, tmp.size()-countE-countS);

	for(int i=0 ; i<str.size() ; i++)
	{
		if( (str[i] == ' ' ) || 
			(str[i] == '\t') )
			str[i]	= '_';
	}
}

void QuerryProcessor::ProcessInRealTimeMode()
{
	//[Y] <your-query> [author=XXX,YYY,...] [category=XXX,YYY,..] [�] �
	while(1)
	{
		cout << "Enter you query Or Enter \"stop\" to exit : " ;
		string data;
		getline(cin, data);

		if(data == "stop")
		{
			break;
		}


		string::size_type loc1 = string::npos;
		string::size_type loc2 = string::npos;
		string::size_type loc3 = string::npos;
		string::size_type loc4 = string::npos;
		string::size_type loc5 = string::npos;
		string::size_type loc6 = string::npos;
		string::size_type loc7 = string::npos;
		string::size_type loc8 = string::npos;

		string srchStr1	= "[";
		string srchStr2	= "]";
		string srchStr3	= "<";
		string srchStr4	= ">";
		string srchStr5	= "[author=";
		string srchStr6	= "]";
		string srchStr7	= "[category=";
		string srchStr8	= "]";

		string	noOfResults, query, author, category;

		loc1	= data.find(srchStr1,    0);
		loc2	= data.find(srchStr2, loc1);
		loc3	= data.find(srchStr3, loc2);
		loc4	= data.find(srchStr4, loc3);
		loc5	= data.find(srchStr5, loc4);
		loc6	= data.find(srchStr6, loc5);
		loc7	= data.find(srchStr7, loc6);
		loc8	= data.find(srchStr8, loc7);

		if( (loc1 < loc2 && loc2 < loc3 && loc3 < loc4 && loc4 < loc5 && loc5 < loc6 && loc6 < loc7 && loc7 < loc8) &&
			(loc1 != string::npos && loc2 != string::npos && loc3 != string::npos && loc4 != string::npos && loc5 != string::npos &&
			 loc6 != string::npos && loc7 != string::npos)) 
		{
			// do nothing
		}
		else
		{
			cout << "\nInvalid Input." << endl;
			cout << "Sample querry format : " << endl;
			cout << "	[Y] <your-query> [author=XXX,YYY] [category=XXX,YYY]" << endl << endl;
			continue;
		}

		// extract num
		noOfResults.append(data, loc1+srchStr1.size(), loc2-loc1-srchStr1.size());
		// extract query
		query.append(data, loc3+srchStr3.size(), loc4-loc3-srchStr3.size());
		// extract author
		author.append(data, loc5+srchStr5.size(), loc6-loc5-srchStr5.size());
		// extract category
		category.append(data, loc7+srchStr7.size(), loc8-loc7-srchStr7.size());

		QueryUnit queryUnit;
		queryUnit.m_numOfResults	= noOfResults;
		AddTermsToQueryVector(query, queryUnit.m_queryVector);
		AddTokensToVector(author, queryUnit.m_author);
		AddTokensToVector(category, queryUnit.m_category);

		vector<DocumentScore> result	= m_IREngine.ProcessQuery(queryUnit);

		DisplayResults(result, queryUnit);
	}
}

void QuerryProcessor::DisplayResults(vector<DocumentScore>& results, QueryUnit& queryUnit)
{
	int rank	= 0;

	cout << endl;
	for(vector<DocumentScore>::iterator it = results.begin() ; it != results.end() ; it++)
	{
		rank++;
		DocumentScore &tmp	= *it;
		string docPath		= m_IREngine.GetFileNameUsingDocId(tmp.m_docId);
		string semWikiDocPath;
		string semWikiSnippet;

		stringstream str;
		str << rank << "\t" << tmp.m_score << "\t";
		str << docPath << endl;

		if(m_IREngine.IsThisAWikiDocument(docPath))
		{
			semWikiDocPath	= m_IREngine.GetFileNameUsingDocId(tmp.m_docId+1);
			semWikiSnippet	= GetStaticSnippet(tmp.m_docId);
			str << semWikiSnippet << endl;
		}
		cout << endl;

		//vector< size_t > termPositions		= GetTermPositionFromFwdIndex(queryUnit);
		//vector< pair<size_t, size_t> > clstr	= GetTermClusters(termPositions);
		//vector< string > snippetVector		= GetSnippets(clstr);

		cout << str.str();
	}
	cout << endl;
}

string QuerryProcessor::GetStaticSnippet(size_t docId)
{
	string snippet;
	string tmpSnip;
	
	string fileName			= m_IREngine.GetFileNameUsingDocId(docId);
	string::size_type loc	= string::npos;
	loc	= fileName.find("/wiki/", 0);

	if(loc == string::npos)
	{
		// news file
		return snippet;
	}
	
	// else get semwiki file name
	string semWikifileName	= m_IREngine.GetFileNameUsingDocId(docId+1);	// semwiki file is the next doc that is pushed in the file dict

	ifstream fp;
	fp.open(semWikifileName.c_str());
	if(!fp)
	{
		cout << "Error : can not open SemiWiki file : " << semWikifileName << endl;
	}

	string line;
	string semiWikiFile;
	while ( !fp.eof() ) 
	{
		getline( fp, line );
		semiWikiFile	+= line;
	}

	loc		= semiWikiFile.find("<#SECTIONS>", 0);
	tmpSnip.append(semiWikiFile, loc+1, SEMI_WIKI_SNIPPET_CHAR_LENGTH);
	loc		= tmpSnip.find("$", 0);
	snippet.append(tmpSnip, loc+1, tmpSnip.size() - (loc+1));
	snippet.append("...");

	fp.close();

	return snippet;
}

void QuerryProcessor::TokenizeString(const string& in_target, const string& in_delims, vector< string >& out_vector)
{
  	string::size_type lastPos = in_target.find_first_not_of(in_delims, 0);
	string::size_type pos     = in_target.find_first_of(in_delims, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{
		string str = in_target.substr(lastPos, pos - lastPos);
		out_vector.push_back(str);
		lastPos = in_target.find_first_not_of(in_delims, pos);
		pos     = in_target.find_first_of(in_delims, lastPos);
	}
}

void toLowerCase(string& target)
{
	for(string::iterator it = target.begin(); it != target.end(); it++)
		*it = tolower(*it);
}

void QuerryProcessor::AddTokensToVector(string str, vector<string> &vec)
{
	vector<string> vec1;
	TokenizeString(str,",", vec1);

	for(int i=0 ; i< vec1.size() ; i++)
	{
		ExtractMainName(vec1[i]);
		toLowerCase(vec1[i]);
		if(vec1[i] != "")
		{
			vec.push_back(vec1[i]);
		}
	}
}

void QuerryProcessor::AddTermsToQueryVector(string str, vector<QueryTerm>& queryVec)
{
	vector<string> vec;
	vector<string> vec1;
	TokenizeString(str,"  \n\r\t\f,", vec1);

	for(int i=0 ; i< vec1.size() ; i++)
	{
		ExtractMainName(vec1[i]);
		toLowerCase(vec1[i]);
		if(vec1[i] != "")
		{
			vec.push_back(vec1[i]);
		}
	}

	map<string, size_t> queryMap;

	for(vector<string>::iterator it = vec.begin() ; it != vec.end() ; it++)
	{
		if((*it).size() == 0)
			continue;

		if(queryMap.find(*it) == queryMap.end())
		{
			queryMap[*it]	= 1;
		}
		else
		{
			queryMap[*it]	+= 1;
		}
	}

	for(map<string, size_t>::iterator it = queryMap.begin() ; it != queryMap.end() ; it++)
	{
		QueryTerm tmp;
		tmp.m_term	= (*it).first;
		tmp.m_freq	= (*it).second;
		queryVec.push_back(tmp);
	}
}

string QuerryProcessor::ExtractFileName(string filePath)
{
	string fileName;

	string::size_type loc1 	= 0;
	string::size_type loc2  = 0;
	while (loc1 != string::npos)
	{
		loc2	= loc1;
		loc1	= filePath.find("/", loc2+1);
	}

	fileName.append(filePath, loc2+1, filePath.size()-loc2-1);

	return fileName;
}

void QuerryProcessor::AppendResultToFile(string queryNo, vector<DocumentScore>& result, string &resultStr)
{
	stringstream str;
	int rank	= 0;

	for(vector<DocumentScore>::iterator it = result.begin() ; it != result.end() ; it++)
	{
		DocumentScore &tmp	= *it;
		string filePath		= m_IREngine.GetFileNameUsingDocId(tmp.m_docId);
		string fileName		= ExtractFileName(filePath);
		rank++;
		ExtractMainName(queryNo);
		str << queryNo << "\t" << 0 << "\t" << fileName << "\t" << rank << "\t" << tmp.m_score << "\t" << "Lost" << endl;
	}

	resultStr	= str.str();
}

vector<BatchQuerryUnit> QuerryProcessor::ParseBatchFile(string batchFile)
{
	vector<BatchQuerryUnit>	queries;
	
	ifstream fp(batchFile.c_str());
	if(!fp)
	{
		cout << "Error : Can not open file for reading queries." << endl;
		return queries;
	}
	string data;
	string line;
	while ( !fp.eof() ) 
	{
		getline( fp, line );
		data += line + '\n';
	}

	string::size_type loc1 = string::npos;
	string::size_type loc2 = string::npos;
	string::size_type loc3 = string::npos;
	string::size_type loc4 = string::npos;

	string	sSrchStr1, sSrchStr2, sSrchStr3;
	string	sTempStr;

	loc1	= 0;
	loc2	= 0;
	while(1)
	{
		// extract top
		sSrchStr1	= "<top>";
		sSrchStr2	= "</top>";
		// add to inverted indexes
		vector<string> vecStr1;
		loc1		= data.find(sSrchStr1, loc2);
		if (loc1 != string::npos)
		{
			loc2	= data.find(sSrchStr2, loc1+sSrchStr1.size());

			if (loc2 != string::npos)
			{
				sTempStr.clear();
				sTempStr.append(data, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());
			}
		}
		
		if(loc1 == string::npos || loc2 == string::npos)
		{
			break;
		}

		BatchQuerryUnit unit;

		// extract <num>
		sSrchStr1	= "<num> Number:";
		sSrchStr2	= "<title>";
		loc3	= 0;
		loc4	= 0;
		loc3		= sTempStr.find(sSrchStr1, loc4);
		if (loc3 != string::npos)
		{
			loc4	= sTempStr.find(sSrchStr2, loc3+sSrchStr1.size());

			if (loc4 != string::npos)
			{
				unit.m_num.append(sTempStr, loc3+sSrchStr1.size(), loc4-loc3-sSrchStr1.size());
			}
		}
		// extract <title>
		sSrchStr1	= "<title>";
		sSrchStr2	= "<desc> Description:";
		loc3	= 0;
		loc4	= 0;
		loc3		= sTempStr.find(sSrchStr1, loc4);
		if (loc3 != string::npos)
		{
			loc4	= sTempStr.find(sSrchStr2, loc3+sSrchStr1.size());

			if (loc4 != string::npos)
			{
				unit.m_title.append(sTempStr, loc3+sSrchStr1.size(), loc4-loc3-sSrchStr1.size());
			}
		}
		// extract <desc>
		sSrchStr1	= "<desc> Description:";
		sSrchStr2	= "<narr> Narrative:";
		loc3	= 0;
		loc4	= 0;
		loc3		= sTempStr.find(sSrchStr1, loc4);
		if (loc3 != string::npos)
		{
			loc4	= sTempStr.find(sSrchStr2, loc3+sSrchStr1.size());

			if (loc4 != string::npos)
			{
				unit.m_desc.append(sTempStr, loc3+sSrchStr1.size(), loc4-loc3-sSrchStr1.size());
			}
		}
		// extract <narr>
		sSrchStr1	= "<narr> Narrative:";
		sSrchStr2	= "</top>";
		loc3	= 0;
		loc4	= 0;
		loc3		= sTempStr.find(sSrchStr1, loc4);
		if (loc3 != string::npos)
		{
			loc4	= sTempStr.size();

			if (loc4 != string::npos)
			{
				unit.m_narr.append(sTempStr, loc3+sSrchStr1.size(), loc4-loc3-sSrchStr1.size());
			}
		}

		queries.push_back(unit);
	}

	return queries;
}

void QuerryProcessor::printfiletoauthormap(map<string, string> F2Amap)
{
	ofstream ofile;
	string opfile = m_params.path_dict_dir + "/F2A.txt";
	ofile.open(opfile.c_str());
	for(map<string, string>::iterator it = F2Amap.begin(); it != F2Amap.end(); it++)
	{
		ofile << it->first << "$"<< it->second << "\n";
	}	
	ofile.close();
}


void QuerryProcessor::dumpAuthorLinks() 
{

	//open auhor inv index file
	ifstream ifile;
	string authinvindxfile = m_params.path_auth_inv_index_dir+"/index.inv.0";
	string authpairsfile = m_params.path_dict_dir+"/authorpairs.txt";
	string linkpairsfile = m_params.path_dict_dir + "/LinkRepository.txt";
	ifile.open(authinvindxfile.c_str());
	map<string, string> FileNToAuthorNMap;
	while(!ifile.eof())
	{
		string authorname;
		int unused1, unused2;
		ifile >> authorname;
		ifile >> unused1 >> unused2;
		list<PostingValue> PostingList;
		(m_IREngine.m_srchAuthInvIndx)->GetPostingList(authorname, PostingList);//get postings list using function
		for(list<PostingValue>::iterator it = PostingList.begin(); it != PostingList.end(); it++)
		{
			//cout << (*it).m_iDocId << "\t";
			string filenamepath = m_IREngine.GetFileNameUsingDocId((*it).m_iDocId); //get fileNameusingDocID. this function returns path not filename
			string filename = "DEFAULT_FILENAME";
			filename = GetFileName(filenamepath);
			//cout << authorname << " is author of " << filename << " at " << filenamepath << endl;
			FileNToAuthorNMap[filename] = authorname;	//make filename to author name map in memory
		}
	}

	printfiletoauthormap(FileNToAuthorNMap);
	
	ifstream ifile2;
	ofstream ofile;
	ofile.open(authpairsfile.c_str());
	ifile2.open(linkpairsfile.c_str()); 	//open link pair file and read it in memory
	string file1, file2, author1, author2;
	map<string, string> AtoAmap;
	while(!ifile2.eof())	//make author to author map and dump simultaneously
	{
		ifile2 >> file1; 
		ifile2 >> file2;
		//cout << "file1 = " << file1 << " file2 = " << file2 << endl;
		map<string, string>::iterator it1 = FileNToAuthorNMap.find(file1);
		if(it1 == FileNToAuthorNMap.end())
			author1 = "UNKNOWN";
		else
			author1 = it1->second;
		map<string, string>::iterator it2 = FileNToAuthorNMap.find(file2);
		if(it2 == FileNToAuthorNMap.end())
			author2 = "UNKNOWN";
		else
			author2 = it2->second;
		AtoAmap[author1] = author2;
		//cout << "author1 = " << author1 << " author2 = " <<author2;
		if(!(author1 == "UNKNOWN" || author2 == "UNKNOWN"))
			ofile << author1 << " , " << author2 << "\n";
	}

	FileNToAuthorNMap.clear();
	AtoAmap.clear();
	ifile.close();
	ifile2.close();
	ofile.close();
}

string QuerryProcessor::GetFileName(string filePath)
{
	string fileName;
	string::size_type loc1 = 0;
	string::size_type loc2 = 0;
	string::size_type loc3 = 0;

	do
	{
		loc2	= loc1;
		loc1	= filePath.find("/", loc1+1);
	}while(loc1 != string::npos);
	
	if(loc2 != 0)
	{
		loc1	= 0;
		do
		{
			loc3	= loc1;
			loc1	= filePath.find(".", loc1+1);
		}while(loc1 != string::npos);

		if(loc3 != 0)
		{
			fileName.append(filePath, loc2+1, loc3-loc2-1);
		}
		else
		{
			fileName.append(filePath, loc2+1, filePath.size()-(loc2+1));
		}
	}

	return fileName;
}
