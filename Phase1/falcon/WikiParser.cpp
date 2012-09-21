/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#include "WikiParser.h"

WikiParser::WikiParser(string outputDirectory):
m_sOutputDirectory(outputDirectory)
{
	m_AuthorInvIndx.SetDataMembers(outputDirectory + "/TempAuthorInvIndx/index.inv.", NO_OF_AUTHOR_INV_INDX_BARRELS, NO_OF_AUTHORS_PER_BARREL);
	m_CategoryInvIndx.SetDataMembers(outputDirectory + "/TempCategoryInvIndx/index.inv.", NO_OF_CATEGORY_INV_INDX_BARRELS, NO_OF_CATEGORIES_PER_BARREL);
}

WikiParser::~WikiParser()
{
}

string GetFileName(string filePath)
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


bool WikiParser::PreProcessDocument(Document &doc, Document &semWikiDoc, bool dumpMetadata, vector<string> &authVector, vector<string> &catgVector)
{
	// clear any garbage data
	semWikiDoc.ClearData();

	ExtractMetadata(doc.m_sPath, doc.m_iDocID, doc.m_sData, semWikiDoc.m_sData, authVector, catgVector);

	if(dumpMetadata)
	{
		semWikiDoc.m_sPath	= m_sOutputDirectory + "/SemWiki";
		semWikiDoc.m_sPath.append("/");

		stringstream id;
		id << GetFileName(doc.m_sPath);
		id << "_semwiki_meta.txt";
		semWikiDoc.m_sPath.append(id.str());
		
		DumpMetadata(semWikiDoc.m_sPath, semWikiDoc.m_sData);
	}

	return true;
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
			(tmp[i] == '\t') )
			countS++;
		else
			break;
	}

	int countE	= 0;
	for(int i=str.size()-1 ; i>=0 ; i--)
	{
		if( (tmp[i] == ' ' ) || 
			(tmp[i] == '\t') )
			countE++;
		else
		{
			/*if(countE == 0)
				countE = str.size();*/
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

void toLowerCase(string& target)
{
	for(string::iterator it = target.begin(); it != target.end(); it++)
		*it = tolower(*it);
}

bool GetDstDoc(string &dst)
{
	for(int i=0 ; i<dst.size() ; i++)
	{
		if( (dst[i] == '\n') ||
			(dst[i] == '<')  ||
			(dst[i] == '>')  ||
			(dst[i] == '{')  ||
			(dst[i] == '}'))
			return false;

		if( (dst[i] == ' ') ||
			(dst[i] == '\t') )
			dst[i] = '_';
	}
	return true;
}

bool WikiParser::ExtractMetadata(string docName, size_t docID, string &docData, string &semWikiData, vector<string> &authVector, vector<string> &catgVector)
{
	// clean 
	m_sMetadata.clear();
	m_sMetadata_sw.clear();

	string::size_type loc1 = string::npos;
	string::size_type loc2 = string::npos;
	string::size_type loc3 = string::npos;
	string::size_type loc4 = string::npos;
	string::size_type loc5 = string::npos;

	string	sSrchStr1, sSrchStr2, sSrchStr3;
	string	sTempStr;

	// count no of references
	//m_sRefAndSectionCount
	int noOfRefs 	= 0;
	sSrchStr1	= "<ref>";
	loc1		= 0;
	while(1)
	{
		loc1	= docData.find(sSrchStr1, loc1+sSrchStr1.size());

		if (loc1 != string::npos)
		{
			noOfRefs++;
		}
		else
		{
			break;
		}
	}
	// count no of sections
	int noOfSecs	= 0;


	// Article name
	m_sMetadata.append("<#ARTICLE_NAME>\n");
	m_sMetadata_sw.append("<#ARTICLE_NAME>\n");
	m_sMetadata.append(docName);
	m_sMetadata_sw.append(docName);
	m_sMetadata.append("\n\n");
	m_sMetadata_sw.append("\n\n");

	// extract Author
	sSrchStr1	= "<<Author>>:";
	sSrchStr2	= "\n";
	// add to inverted indexes
	vector<string> vecStr1;
	loc1		= docData.find(sSrchStr1, 0);
	if (loc1 != string::npos)
	{
		loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());

		if (loc2 != string::npos)
		{
			sTempStr.clear();
			sTempStr.append(docData, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());
			m_dAuthDict.AddNewElementIfNotPresent(sTempStr);
			ExtractMainName(sTempStr);
			toLowerCase(sTempStr);
			m_sMetadata.append("<#AUTHOR>\n");
			m_sMetadata_sw.append("<#AUTHOR>\n");
			m_sMetadata.append(sTempStr);
			m_sMetadata_sw.append(sTempStr);
			m_sMetadata.append("\n");
			m_sMetadata_sw.append("\n");
			
			// add to inverted indexes
			authVector.push_back(sTempStr);
		}
	}
	//if(vecStr1.size())
	//	m_AuthorInvIndx.AddData(docID, vecStr1);
	m_sMetadata.append("\n");
	m_sMetadata_sw.append("\n");

	// extract Timestamp
	sSrchStr1	= "<<Timestamp>>:";
	sSrchStr2	= "\n";
	loc1		= docData.find(sSrchStr1, 0);
	if (loc1 != string::npos)
	{
		loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());

		if (loc2 != string::npos)
		{
			m_sMetadata.append("<#TIMESTAMP>\n");
			m_sMetadata_sw.append("<#TIMESTAMP>\n");
			m_sMetadata.append(docData, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());
			m_sMetadata_sw.append(docData, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());
			m_sMetadata.append("\n");
			m_sMetadata_sw.append("\n");
		}
	}
	m_sMetadata.append("\n");
	m_sMetadata_sw.append("\n");

	// extract Infobox
	sSrchStr1	= "{{Infobox";
	sSrchStr2	= "}}";
	sSrchStr3	= "{{";
	loc1		= docData.find("{{Infobox", 0);
	if (loc1 != string::npos)
	{
		loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());
		loc3	= docData.find(sSrchStr3, loc1+sSrchStr1.size());
		// take care of multiple {{ {{ }} }} case
		while(loc3 < loc2 && loc3 != string::npos)
		{
			loc2	= docData.find(sSrchStr2, loc2+sSrchStr2.size());
			loc3	= docData.find(sSrchStr3, loc3+sSrchStr3.size());
		}

		if (loc2 != string::npos)
		{
			m_sMetadata.append("<#INFOBOX>\n");
			m_sMetadata_sw.append("<#INFOBOX>\n");
			m_sMetadata.append(docData, loc1, loc2-loc1+sSrchStr2.size());
			m_sMetadata_sw.append(docData, loc1, loc2-loc1+sSrchStr2.size());
			m_sMetadata.append("\n\n");
			m_sMetadata_sw.append("\n\n");
		}
	}

	// extract Taxobox
	sSrchStr1	= "{{Taxobox";
	sSrchStr2	= "}}";
	sSrchStr3	= "{{";
	loc1		= docData.find("{{Taxobox", 0);
	if (loc1 != string::npos)
	{
		loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());
		loc3	= docData.find(sSrchStr3, loc1+sSrchStr1.size());
		// take care of multiple {{ {{ }} }} case
		while(loc3 < loc2 && loc3 != string::npos)
		{
			loc2	= docData.find(sSrchStr2, loc2+sSrchStr2.size());
			loc3	= docData.find(sSrchStr3, loc3+sSrchStr3.size());
		}

		if (loc2 != string::npos)
		{
			m_sMetadata.append("<#TAXOBOX>\n");
			m_sMetadata_sw.append("<#TAXOBOX>\n");
			m_sMetadata.append(docData, loc1, loc2-loc1+sSrchStr2.size());
			m_sMetadata_sw.append(docData, loc1, loc2-loc1+sSrchStr2.size());
			m_sMetadata.append("\n\n");
			m_sMetadata_sw.append("\n\n");
		}
	}

	// extract sections
	bool addSection	= true;
	m_sMetadata.append("<#SECTIONS>\n");
	m_sMetadata_sw.append("<#SECTIONS>\n");
	sSrchStr1	= "==";
	sSrchStr2	= "=";
	loc2		= 0;
	do
	{
		loc1	= docData.find(sSrchStr1, loc2+sSrchStr1.size()*2);		// *2 to take care of === and ==== cases

		if (loc1 != string::npos)
		{
			loc2	= docData.find(sSrchStr1, loc1+sSrchStr1.size()*2);	

			if (loc2 != string::npos)
			{
				sTempStr.clear();
				sTempStr.append(docData, loc1, loc2-loc1);
				// remove starting =
				do
				{
					loc3	= sTempStr.find(sSrchStr2, 0);
					if(loc3 == 0)
					{
						sTempStr.erase(loc3, 1);
					}
				}
				while(loc3 == 0);

				m_sMetadata.append(sTempStr);
				m_sMetadata_sw.append(sTempStr);
				m_sMetadata.append(" $ ");
				m_sMetadata_sw.append(" $ ");
				noOfSecs++;
				
				// append atmost 20 words
				loc3	= docData.find(sSrchStr1, loc2+sSrchStr1.size()*2);	
				if(loc3 != string::npos)
				{
					sTempStr.clear();
					sTempStr.append(docData, loc2, loc3-loc2);
					// remove starting =
					do
					{
						loc3	= sTempStr.find(sSrchStr2, 0);
						if(loc3 == 0)
						{
							sTempStr.erase(loc3, 1);
						}
					}
					while(loc3 == 0);
				}
				RemoveMarkupData(sTempStr);
				m_sMetadata.append(sTempStr);
				m_sMetadata.append("\n");
				if(addSection)
				{
					m_sMetadata_sw.append(sTempStr);
					m_sMetadata_sw.append("\n");
				}
			}
		}
	}while(loc1 != string::npos && loc2 != string::npos);
	m_sMetadata.append("\n");
	m_sMetadata_sw.append("\n");

	// extract links 
	string src	= GetFileName(docName);
	string dst;
	m_sMetadata.append("<#LINKS>\n");
	m_sMetadata_sw.append("<#LINKS>\n");
	sSrchStr1	= "[[";
	sSrchStr2	= "]]";
	sSrchStr3	= "|";
	loc2		= 0;
	loc3		= docData.find("[[Category:", 0);
	do
	{
		loc1	= docData.find(sSrchStr1, loc2);

		if (loc1 != string::npos)
		{
			loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());

			if (loc2 != string::npos && loc2 < loc3)
			{
				sTempStr.clear();
				sTempStr.append(docData, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());

				// check for link for external file :
				loc4	= 0;
				loc5	= 0;
				if(sTempStr.find(":", loc5) != string::npos)
				{
					continue;
				}

				// check for nested links [[ [[ ]] ]] 
				loc4	= 0;
				loc5	= 0;
				do
				{	
					// go to last [[
					loc5	= loc4;
					loc4	= sTempStr.find(sSrchStr1, loc5+sSrchStr1.size());
				}while(loc4 != string::npos);
				
				if(loc5 != string::npos && loc5 != 0)
				{
					sTempStr.erase(0, loc5+sSrchStr1.size());
				}

				// check for multiple OR statements [[ | | ]] 
				loc4	= 0;
				loc5	= 0;
				do
				{	
					// go to last [[
					loc5	= loc4;
					loc4	= sTempStr.find(sSrchStr3, loc5+sSrchStr3.size());
				}while(loc4 != string::npos);
				
				if(loc5 != string::npos && loc5 != 0)
				{
					sTempStr.erase(0, loc5+sSrchStr3.size());
				}

				if(sTempStr.size() > 2)
				{
					m_sMetadata.append(sTempStr);
					m_sMetadata_sw.append(sTempStr);
					m_sMetadata.append(" $ ");
					m_sMetadata_sw.append(" $ ");
					dst	= sTempStr;

					// add to link repository
					if(GetDstDoc(dst))
					{
						m_sLinkRepository.append(src + " " + dst + "\n");
					}
				}
			}
		}
	}while(loc1 != string::npos && loc2 != string::npos && loc2 < loc3);
	m_sMetadata.append("\n\n");
	m_sMetadata_sw.append("\n\n");

	// extract category
	m_sMetadata.append("<#CATEGORY>\n");
	m_sMetadata_sw.append("<#CATEGORY>\n");
	sSrchStr1	= "[[Category:";
	sSrchStr2	= "]]";
	loc2		= 0;
	// add to inverted indexes
	vector<string> vecStr2;
	do
	{
		loc1	= docData.find(sSrchStr1, loc2);

		if (loc1 != string::npos)
		{
			loc2	= docData.find(sSrchStr2, loc1+sSrchStr1.size());

			if (loc2 != string::npos)
			{
				sTempStr.clear();
				sTempStr.append(docData, loc1+sSrchStr1.size(), loc2-loc1-sSrchStr1.size());

				// check for link for external file :
				loc4	= 0;
				loc5	= 0;
				if(sTempStr.find(":", loc5) != string::npos)
				{
					continue;
				}

				// check for nested links [[ [[ ]] ]] 
				loc4	= 0;
				loc5	= 0;
				do
				{	
					// go to last [[
					loc5	= loc4;
					loc4	= sTempStr.find(sSrchStr1, loc5+sSrchStr1.size());
				}while(loc4 != string::npos);
				
				if(loc5 != string::npos && loc5 != 0)
				{
					sTempStr.erase(0, loc5+sSrchStr1.size());
				}

				// check for multiple OR statements [[ | | ]] 
				loc4	= 0;
				loc5	= 0;
				do
				{	
					// go to last [[
					loc5	= loc4;
					loc4	= sTempStr.find(sSrchStr3, loc5+sSrchStr3.size());
				}while(loc4 != string::npos);
				
				if(loc5 != string::npos && loc5 != 0)
				{
					sTempStr.erase(0, loc5+sSrchStr3.size());
				}

				bool isAdded	= m_dCatgDict.AddNewElementIfNotPresent(sTempStr);
				if(isAdded)
				{
					m_sMetadata.append(sTempStr);
					m_sMetadata_sw.append(sTempStr);
					if(sTempStr != " ")
					{
						ExtractMainName(sTempStr);
						toLowerCase(sTempStr);
						m_sMetadata.append(" $ ");
						m_sMetadata_sw.append(" $ ");
						catgVector.push_back(sTempStr);
					}
				}
			}
		}
	}while(loc1 != string::npos && loc2 != string::npos);
	//if(vecStr1.size())
	//	m_CategoryInvIndx.AddData(docID, vecStr2);
	m_sMetadata.append("\n\n");
	m_sMetadata_sw.append("\n\n");

	// copy sem wiki data
	docData		= m_sMetadata;
	semWikiData	= m_sMetadata_sw;

	// add ref and sec count
	stringstream tmpStr;
	tmpStr << GetFileName(docName);
	tmpStr << "$";
	tmpStr << noOfRefs;
	tmpStr << "$";
	tmpStr << noOfSecs;
	tmpStr << "\n";
	m_sRefAndSectionCount.append(tmpStr.str());

	return true;
}

bool WikiParser::DumpMetadata(string &dumpPath, string &data)
{
	ofstream semWiki;
	semWiki.open(dumpPath.c_str());
	if (!semWiki) 
	{
		cout << "Error : Can not open semWiki file for dumping metadata." << endl;
	}

	semWiki << data.c_str();
	semWiki.close();

	return true;
}

bool WikiParser::DumpDictionaries()
{
	string tempStr;

	tempStr	= m_sOutputDirectory + "/Dictionaries";
	tempStr.append("/AuthorDict.txt");
	m_dAuthDict.DumpDictionary(tempStr);

	tempStr	= m_sOutputDirectory + "/Dictionaries";
	tempStr.append("/CategoryDict.txt");
	m_dCatgDict.DumpDictionary(tempStr);

	tempStr	= m_sOutputDirectory + "/Dictionaries";
	tempStr.append("/LinkRepository.txt");
	ofstream linkRepo;
	linkRepo.open(tempStr.c_str());
	if (!linkRepo) 
	{
		cout << "Error : Can not open linkRepo file for dumping link repository." << endl;
	}
	linkRepo << m_sLinkRepository;
	linkRepo.close();

	tempStr	= m_sOutputDirectory + "/Dictionaries";
	tempStr.append("/RefAndSecCountRepository.txt");
	ofstream refAndSecRepo;
	refAndSecRepo.open(tempStr.c_str());
	if (!refAndSecRepo)
	{
		cout << "Error : Can not open refAndSecRepo file for dumping refAndSec repository." << endl;
	}
	refAndSecRepo << m_sRefAndSectionCount;
	refAndSecRepo.close();

	return true;
}

bool WikiParser::DumpRemainingInvIndx()
{
	m_AuthorInvIndx.DumpInvertedIndex();
	m_CategoryInvIndx.DumpInvertedIndex();

	return true;
}

bool RemoveDataWithin(string &data, string &sSrchStr1 ,string &sSrchStr2)
{
	string::size_type loc1	= string::npos;
	string::size_type loc2	= string::npos;

	loc2		= 0;
	do
	{
		loc1	= data.find(sSrchStr1, loc2);

		if (loc1 != string::npos)
		{
			loc2	= data.find(sSrchStr2, loc1+sSrchStr1.size());

			if (loc2 != string::npos)
			{
				data.erase(loc1, loc2-loc1+sSrchStr2.size());
				// update loc2 as some of the characters have been deleteds
				loc2	= loc1;
			}
		}
	}while(loc1 != string::npos && loc2 != string::npos);

	return true;
}

bool RemoveString(string &data, string &sSrchStr)
{
	string::size_type loc1	= string::npos;

	loc1	= 0;
	do
	{
		loc1	= data.find(sSrchStr, loc1);

		if (loc1 != string::npos)
		{
				data.erase(loc1, sSrchStr.size());
		}
	}while(loc1 != string::npos);

	return true;
}

bool WikiParser::RemoveMarkupData(string &data)
{
	string::size_type loc1 = string::npos;
	string::size_type loc2 = string::npos;

	vector<string> vSrchStrStrt;  
	vector<string> vSrchStrEnds;

	// patterns
	// 1 {{*}}
	vSrchStrStrt.push_back("{{");
	vSrchStrEnds.push_back("}}");
	// 2 <*>
	vSrchStrStrt.push_back("<");
	vSrchStrEnds.push_back(">");
	// 3 [[*]]
	//vSrchStrStrt.push_back("[[");
	//vSrchStrEnds.push_back("]]");
	// 4 "[*]"
	//vSrchStrStrt.push_back("[");
	//vSrchStrEnds.push_back("]");
	// 5 "&*;"
	vSrchStrStrt.push_back("&");
	vSrchStrEnds.push_back(";");
	// 6 "http:* "
	vSrchStrStrt.push_back("http:");
	vSrchStrEnds.push_back(" ");
	// 7 "www.* "
	vSrchStrStrt.push_back("www.");
	vSrchStrEnds.push_back(" ");

	int noOfPatterns	= vSrchStrStrt.size();

	for(int i=0 ; i<noOfPatterns ; i++)
	{
		RemoveDataWithin(data, vSrchStrStrt[i] ,vSrchStrEnds[i]);
	}

	vector<string> vSrchStr; 

	// 1 [[
	vSrchStr.push_back("[[");
	// 2 ]]
	vSrchStr.push_back("]]");
	// 3 \n
	vSrchStr.push_back("\n");
	// 4 *
	vSrchStr.push_back("*");
	// 5 |
	vSrchStr.push_back("|");

	noOfPatterns	= vSrchStr.size();

	for(int i=0 ; i<noOfPatterns ; i++)
	{
		RemoveString(data, vSrchStr[i]);
	}

	return false;
}
