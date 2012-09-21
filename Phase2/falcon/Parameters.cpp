#include "Parameters.h"

Parameters::Parameters()
{
}

Parameters::Parameters(const char* filePath)
{	
	vector<string> para;
	string data;
	ifstream ifile(filePath);
	while(getline(ifile, data, ','))
		para.push_back(trim(data));

	path_auth_inv_index_dir = para[0];
	path_catg_inv_index_dir = para[1];
	path_dict_dir			= para[2];
	path_fwd_index_dir		= para[3];
	path_inv_index_dir		= para[4];
	path_semi_wiki_dir		= para[5];
	inverted_index_barrels	= atoi(para[6].c_str());
	m_mode					= para[7];
	m_zone					= para[8];
}

string Parameters::trim(const string& data)
{
	string::size_type start = data.find_first_not_of(" ");
	return data.substr(start, string::npos);
}

void Parameters::disp(vector<string> para)
{
	for(int i=0;i<para.size();i++)
	{
		cout << para[i];
	}
}
