/*
Author			: Raman Sonkhla
UB Person No	: 50026724 
*/

#include "DataCollector.h"

// Document

Document::Document(string path, size_t docID, string data):
m_sPath(path), m_iDocID(docID), m_sData(data)
{
}

Document::~Document()
{
}

void Document::ClearData()
{
	m_sPath.clear();
	m_iDocID	= 0;
	m_sData.clear();
}

// ~Document

// DataCollector

DataCollector::DataCollector(string outputDirectory, size_t fileDictBarrelSize):
m_sOutputDirectory(outputDirectory), m_fileDict(outputDirectory, fileDictBarrelSize)
{
}

DataCollector::~DataCollector()
{
}

size_t DataCollector::GetNoOfDocs()
{
	return m_vsFileNames.size();
}

#ifdef MY_WINDOWS
void GetListOfFiles(string parentDirectory, vector<string> &fileList)
{
	WIN32_FIND_DATA FindFileData;
    string sTmpPath = parentDirectory;
    sTmpPath += "/*.*";

    HANDLE hFind = FindFirstFile(sTmpPath.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "Error : Can not access data Directory " << sTmpPath << endl;

        return;
	}
    else 
	{
        do 
		{
            if( ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) 
			{
				// if directory:
                if( strcmp(".", FindFileData.cFileName ) && strcmp("..", FindFileData.cFileName) ) 
				{
					sTmpPath = parentDirectory;
                    sTmpPath += "/";
					sTmpPath += FindFileData.cFileName;
                    GetListOfFiles( sTmpPath.c_str(), fileList);
                }
            }
            else // if file:
            {
				sTmpPath = parentDirectory;
                sTmpPath += "/";
				sTmpPath += FindFileData.cFileName;
				fileList.push_back(sTmpPath);
            }

        } while ( FindNextFile( hFind, &FindFileData) != 0 );

        FindClose( hFind );
    }
}
#else
void GetListOfFiles(string parentDirectory, vector <string> &fileList)
{
	DIR *dirp;
	struct dirent *dp;
	struct stat buf;

	std::string documentpath;
	std::string documentname;

	dirp = opendir(parentDirectory.c_str());
	if (!dirp){
		cout << "Dirp is null" << endl;
		return;
	}

	while ( (dp = readdir(dirp)) ) {
		documentname = dp->d_name;

		documentpath = parentDirectory + "/" + documentname;

		if ( (documentname == ".") || (documentname == "..") )
			continue;

		if (stat(documentpath.c_str(), &buf) == 0)
		{
			// recursively process directories
			if(buf.st_mode & S_IFDIR)
				GetListOfFiles(documentpath, fileList);
			if(!S_ISDIR(buf.st_mode))
			{
				documentpath = parentDirectory + "/" + documentname;
				fileList.push_back(documentpath);
			}
		}
	}
	closedir(dirp);
}
#endif

bool DataCollector::ScanForNewDocuments(string directoryPath)
{
	GetListOfFiles(directoryPath, m_vsFileNames);

	return true;
}

bool DataCollector::GetNextDocument(Document &doc)
{
	// clean garbage
	doc.ClearData();

	static size_t noOfFileAlreadyProcessed	= 0;

	if(noOfFileAlreadyProcessed < m_vsFileNames.size())
	{
		doc.m_sPath		= m_vsFileNames[noOfFileAlreadyProcessed++];
		//doc.m_iDocID	= AddNewDocumentToDict(doc.m_sPath);

		// open file and read its content to doc.data
		ifstream data(doc.m_sPath.c_str());
		string line;

		if ( data.is_open() ) 
		{
			while ( !data.eof() ) 
			{
				getline( data, line );
				doc.m_sData += line + '\n';
			}
		} 
		else 
		{
			cout << "Error: Couldn't open file " << doc.m_sPath.c_str() << endl;
		}
		data.close();

		return true;
	}

	return false;
}

size_t DataCollector::AddNewDocumentToDict(size_t docLen, string path)
{
	return m_fileDict.AddNewElement(docLen, path);
}

bool DataCollector::DumpFileDictionary()
{
	m_fileDict.DumpFileNameBarrel();
	m_fileDict.DumpDictionary();

	return false;
}

size_t DataCollector::GenerateUniqueDocID(string &docName)
{
	static size_t uniqueId	= 0;

	return uniqueId++;
}

// ~DataCollector
