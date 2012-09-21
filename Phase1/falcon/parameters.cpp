#include "Parameters.h"
#include <stdlib.h>

parameters::parameters(const char* filePath)
{	
	vector<string> para;
	string data;
	ifstream ifile(filePath);
	while(getline(ifile, data, ','))
		para.push_back(trim(data));

	path_input_dir = para[0];
	path_index_dir = para[1];
	path_dict_dir = para[2];
	inverted_index_barrels = atoi(para[3].c_str());
	ifile.close();
}

string parameters::trim(const string& data)
{
	string::size_type start = data.find_first_not_of(" ");
	return data.substr(start, string::npos);
}

void parameters::disp(vector<string> para)
{
	for(int i=0;i<para.size();i++)
	{
		cout << para[i];
	}
}
