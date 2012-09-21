#include "TextParser.h"

char* TextParser::stoparr[] = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your"};

TextParser::TextParser(string p)
{
	path = p;
	for(int i = 0; i< 119; i++)
	{
		stoplist.push_back(stoparr[i]);
	}	
	
}

TextParser::~TextParser()
{
}

string TextParser::removeNonAlphaNumericCharactersFromString(string& in_target)
{
	string::iterator it;
	string newString;
	for(it = in_target.begin(); it != in_target.end(); it++)
	{
		if(((*it) > 0) && (isalpha(*it) || (*it == '-') || isspace(*it)))
			newString.push_back(*it);
	}
	return newString;
}

void TextParser::tokenizeString(const string& in_target, const string& in_delims, vector< string >& out_vector)
{
  	string::size_type lastPos = in_target.find_first_not_of(in_delims, 0);
	string::size_type pos     = in_target.find_first_of(in_delims, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{
		string str = in_target.substr(lastPos, pos - lastPos);
		if(isNotStopWord(str))
			out_vector.push_back(str);
		lastPos = in_target.find_first_not_of(in_delims, pos);
		pos     = in_target.find_first_of(in_delims, lastPos);
	}
}

void TextParser::updateTermDictionary(vector <string>& terms, map<string, size_t>& termdict, size_t& currentTermCount)
{
	for(vector<string>::iterator it = terms.begin(); it != terms.end(); it++)
	{
		if(termdict.find(*it) == termdict.end())
		{
			termdict[*it] = currentTermCount + 1;
			currentTermCount++;
		}
	}
}

void TextParser::createWordCountDictionary(vector<string>& in_tokens, map<string, size_t>& out_wordCountDict)
{
	vector<string>::iterator it;

	for(it = in_tokens.begin(); it != in_tokens.end(); it++)
	{
		if(out_wordCountDict.find(*it) == out_wordCountDict.end())
		{
			out_wordCountDict[*it] = 1;
		}
		else
		{
			out_wordCountDict[*it]++;
		}
	}
}

void TextParser::DumpDicts()
{
	TermDict.DumpDictionary(path + "/TermDict.txt");
	m_dWordCountDict.DumpDictionary(path + "/TermCountDict.txt");
}

void TextParser::toLowerCase(string& target)
{
	for(string::iterator it = target.begin(); it != target.end(); it++)
		*it = tolower(*it);
}

bool TextParser::ParseDocument(string& data)
{
	tokens.clear();

	toLowerCase(data);
	tokenizeString(removeNonAlphaNumericCharactersFromString(data)," \n\r\t\f", tokens); //file to tokens
	createWordCountDictionary(tokens,m_dWordCountDict.m_mDict);

	updateTermDictionary(tokens, TermDict.m_mDict, TermDict.m_iIDCounter);
	
	return false;
}

int TextParser::isNotStopWord(string str)
{
	if(find(stoplist.begin(),stoplist.end(),str) == stoplist.end())
		return 1;
	else
		return 0;
}
