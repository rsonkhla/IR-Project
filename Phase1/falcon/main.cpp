//#define MY_WINDOWS // TODO : change it after constructing make file

//#ifdef MY_WINDOWS
#include "DataCollector.h"
#include "DataTypeDetector.h"
#include "Dictionary.h"
#include "TextParser.h"
#include "WikiParser.h"
#include "Indexer.h"
#include "Parameters.h"
#include "TimeAnalysis.h"
#include "fwIndexer.h"
#include "InvertedIndexGenerator.h"
#include "merger.h"
#ifdef DO_NO_INCLUDE
#include "CommonSettingsAndUtilities.cpp"
#include "DataCollector.cpp"
#include "DataTypeDetector.cpp"
#include "Dictionary.cpp"
#include "TextParser.cpp"
#include "WikiParser.cpp"
#include "Indexer.cpp"
#include "parameters.cpp"
#include "TimeAnalysis.cpp"
#include "fwIndexer.cpp"
#include "InvertedIndexGenerator.cpp"
#include "merger.cpp"
#endif
using namespace std;

int main(int argc, char* argv[])
{
	// input
	time_t start = time(NULL);

	#ifdef MY_WINDOWS
	parameters params(argv[1]);
	#else
	parameters params(argv[2]);
	#endif
	string newsDirectoryPath(params.path_input_dir);

	// output
	string outputDirectory(params.path_dict_dir);

	// create output directory
	CreateNewDirectory(outputDirectory);
	CreateNewDirectory(outputDirectory + "/SemWiki");
	CreateNewDirectory(outputDirectory + "/Dictionaries");
	CreateNewDirectory(outputDirectory + "/TempInvIndx");
	CreateNewDirectory(outputDirectory + "/DocInvIndx");
	CreateNewDirectory(outputDirectory + "/DocFwdIndx");
	CreateNewDirectory(outputDirectory + "/TempAuthorInvIndx");
	CreateNewDirectory(outputDirectory + "/AuthorInvIndx");
	CreateNewDirectory(outputDirectory + "/TempCategoryInvIndx");
	CreateNewDirectory(outputDirectory + "/CategoryInvIndx");

	// scan for data
	DataCollector dataCollector(outputDirectory + "/Dictionaries", NO_OF_FILE_PER_BARREL);
	dataCollector.ScanForNewDocuments(newsDirectoryPath);

	// pre-process and parse data
	Document			doc("", 0, "");
	Document			semWiki("", 0, "");
	DataTypeDetector	dataTypeDetector;
	DATA_TYPE			dataType = T_DEFAULT;
	size_t curDocSize	= 0;
	size_t sumDocSize	= 0;

	TextParser textParser(outputDirectory + "/Dictionaries");
	WikiParser wikiParser(outputDirectory);
	bool dumpMetadata	= true;
	
	InvertedIndexGenerator indexer(outputDirectory + "/TempInvIndx", params.inverted_index_barrels, NO_OF_POSTIONG_PER_BARREL);
	//fwIndexer fwi(outputDirectory + "/DocFwdIndx");
	
	while(dataCollector.GetNextDocument(doc))
	{
		// pre-process data 
		dataType	= dataTypeDetector.GetDataType(doc.m_sPath);

		// wiki doc author and category vector
		vector<string> authVector;
		vector<string> catgVector;

		switch(dataType)
		{
			case T_NEWS:
				// do nothing
				break;
			case T_WIKI:
				// remove metadata
				wikiParser.PreProcessDocument(doc, semWiki, dumpMetadata, authVector, catgVector);
				break;
			case T_DEFAULT:
				// throw error
				cout << "Error: Data type detection failed." << endl;
				break;
		}

		// parse data
		textParser.ParseDocument(doc.m_sData);		
		// add document length
		curDocSize	= textParser.tokens.size();
		sumDocSize	+= curDocSize;
		doc.m_iDocID	= dataCollector.AddNewDocumentToDict(curDocSize, doc.m_sPath);
		indexer.AddData(doc.m_iDocID, textParser.tokens);

		if(dataType == T_WIKI)
		{
			// add new sem wiki doc to file dic
			semWiki.m_iDocID	= dataCollector.AddNewDocumentToDict(0, semWiki.m_sPath);
			if(authVector.size())
				wikiParser.m_AuthorInvIndx.AddData(doc.m_iDocID, authVector);
			if(catgVector.size())
				wikiParser.m_CategoryInvIndx.AddData(doc.m_iDocID, catgVector);

			//fwi.makeIndex(semWiki.m_iDocID, semWiki.m_sData, textParser.TermDict.m_mDict);
		}

	
		cout << doc.m_iDocID << endl;
		
	}

	indexer.DumpRemainingInvIndx();
	wikiParser.DumpRemainingInvIndx();
	//fwi.writeIndex();

	string tempInvIndexFolder	= outputDirectory + "/TempInvIndx";
	string invIndexFolder		= outputDirectory + "/DocInvIndx";
	Merger mergeInvIndex((const char*) (tempInvIndexFolder.c_str()), (const char*)(invIndexFolder.c_str()));

	string tempInvIndexFolder1	= outputDirectory + "/TempAuthorInvIndx";
	string invIndexFolder1		= outputDirectory + "/AuthorInvIndx";
	Merger mergeInvIndex1((const char*) (tempInvIndexFolder1.c_str()), (const char*)(invIndexFolder1.c_str()));

	string tempInvIndexFolder2	= outputDirectory + "/TempCategoryInvIndx";
	string invIndexFolder2		= outputDirectory + "/CategoryInvIndx";
	Merger mergeInvIndex2((const char*) (tempInvIndexFolder2.c_str()), (const char*)(invIndexFolder2.c_str()));

	for(int i=0 ; i<params.inverted_index_barrels ; i++)
		mergeInvIndex.mergeIndex(i);
	mergeInvIndex1.mergeIndex(0);
	mergeInvIndex2.mergeIndex(0);

	// dump dictionaries
	cout << "dumping dictionaries\n";
	dataCollector.DumpFileDictionary();
	wikiParser.DumpDictionaries();
	textParser.DumpDicts();
	
	// dump No of docs
	size_t noOfDocs		= dataCollector.GetNoOfDocs();
	size_t avgDocSize	= sumDocSize / noOfDocs;
	ofstream fp;
	fp.open((outputDirectory + "/DocInvIndx" + "/CorpusStats.txt").c_str());
	if(!fp)
	{
		cout << "Error : Can not open file for dumping NoOfDocs." << endl;
	}
	fp << "Average Doc Size : " << avgDocSize << endl;
	fp << "No Of Doc : " << noOfDocs << endl;
	fp.close();

	time_t end = time(NULL);
	cout << "time taken: " << end - start <<"s" <<endl;
	return 0;
}
