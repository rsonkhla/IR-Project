#ifndef TEXT_PARSER
#define TEXT_PARSER

// include
#include "CommonSettingsAndUtilities.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <map>
#include <algorithm>
#include "Dictionary.h"

#ifdef MY_WINDOWS
#else 
#endif

using namespace std;

class TextParser
{
	//functions
public:
	TextParser(string p);
	~TextParser();
	bool ParseDocument(string&	 data);
	
	string removeNonAlphaNumericCharactersFromString(string& in_target);
	void tokenizeString(const string& in_target, const string& in_delims, vector< string >& out_vector);
	void updateTermDictionary(vector <string>& terms, map<string, size_t>& termdict, size_t& currentTermCount);
	
	void createWordCountDictionary(vector<string>& in_tokens, map<string, size_t>& out_wordCountDict);

	void DumpDicts();
	MyDictinary TermDict;
	vector<string> tokens;
	MyDictinary m_dWordCountDict;
	string path;
	int isNotStopWord(string str);
	vector<string> stoplist;//(stoparr, stoparr + 119);
private:
	
	static char *stoparr[];// = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
	void toLowerCase(string& target);
};

#endif
