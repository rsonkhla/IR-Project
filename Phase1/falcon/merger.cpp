

#include "merger.h"

using namespace std;

Merger::Merger(const char* indir, const char* outdir)
{
	// TODO Auto-generated constructor stub
	indexInputDirectory += indir;
	indexOutputDirectory += outdir;

}

Merger::~Merger()
{
	// TODO Auto-generated destructor stub
}

#ifdef MY_WINDOWS
void Merger::getdir(string parentDirectory, vector<string> &fileList)
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
                    getdir( sTmpPath.c_str(), fileList);
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
void Merger::getdir(string parentDirectory, vector <string> &fileList)
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
				getdir(documentpath, fileList);
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

void Merger::mergeIndex(int indexNumber)
{
	vector<string> files;
	vector<string>::iterator it;

	IndexReader ir;

	map<string, ifstream*> filesToBeMerged;

	////cout << "Entered mergeIndex";

	getdir(indexInputDirectory, files);


	char fileName[50];
	sprintf(fileName, "index.inv.%d", indexNumber);
		for(it = files.begin(); it != files.end(); it++)
		{
			if((*it).find(fileName) != string::npos)
			{
				filesToBeMerged[*it] = new ifstream((/*indexInputDirectory + "/" + */*it).c_str());
				////cout << "File " << fileName << "Opened " << endl;
			}
		}
//		/cout << filesToBeMerged.size() << endl;
	ofstream pofile((indexOutputDirectory + "/" + fileName + ".pstngs").c_str());
	ofstream ofsfile((indexOutputDirectory + "/" + fileName).c_str());

	//priority_queue<Posting, vector<Posting>,less<vector<Posting>::value_type> > q;
	priority_queue<Posting, vector<Posting>, PostingComparator> pQueue;
	map<string, Posting> outputBuffer;
	for(map<string, ifstream*>::iterator it1 = filesToBeMerged.begin(); it1 != filesToBeMerged.end(); it1++)
	{

		Posting curPost = ir.readLinesFromFile(*(it1->second), it1->first);
		curPost.fileFrom = it1->first;
		pQueue.push(curPost);

	}

	int offset = 0;
	//cout << "ooopar " << pQueue.size() << endl;
	while(!pQueue.empty())
	{
		//Posting topPost = q.top();


			Posting topPost = pQueue.top();
			pQueue.pop();

			//else
			//{
				if(!((*filesToBeMerged[topPost.fileFrom]).eof()))
				{

					//pQueue.erase(topPost.term);
					Posting curPost = ir.readLinesFromFile(*filesToBeMerged[topPost.fileFrom], topPost.fileFrom.c_str());
					//if(curPost.term == topPost.term)
					//	topPost.mergePostingList(curPost);
					//else
					//{
						pQueue.push(curPost);
					//}
				//}
			}
			if((topPost.term != "") && (!isspace(topPost.term[0])))
			{

				if(outputBuffer.find(topPost.term) != outputBuffer.end())
					outputBuffer[topPost.term].mergePostingList(topPost);
				else
					outputBuffer[topPost.term] = topPost;

				if(outputBuffer.size() >= 20)
				{
					for(map<string, Posting>::iterator it = outputBuffer.begin(); it != outputBuffer.end(); it++)
					{
						ofsfile << it->first << " " << offset << " "<< it->second.postingListAsString.size()<< endl;
					//cout << "Pstngs: " + topPost.postingListAsString << endl;
						pofile << it->second.postingListAsString;
						offset += it->second.postingListAsString.size();
					}
					outputBuffer.clear();
				}
			}
	}
	for(map<string, Posting>::iterator it = outputBuffer.begin(); it != outputBuffer.end(); it++)
						{
							ofsfile << it->first << " " << offset << " "<< it->second.postingListAsString.size()<< endl;
						//cout << "Pstngs: " + topPost.postingListAsString << endl;
							pofile << it->second.postingListAsString;
							offset += it->second.postingListAsString.size();
						}
						outputBuffer.clear();

	for(map<string, ifstream*>::iterator it1 = filesToBeMerged.begin(); it1 != filesToBeMerged.end(); it1++)
		delete it1->second;


}


