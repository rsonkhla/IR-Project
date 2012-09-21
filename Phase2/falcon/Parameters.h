#ifndef PARAMETERS
#define PARAMETERS

#include <iostream>
#include <fstream>		
#include <string>
#include <vector>

#include "CommonSettingsAndUtilities.h"

using namespace std;

class Parameters
{
public:
	Parameters();
	Parameters(const char* filePath);
	
private:
	string trim(const string& data);
	void disp(vector<string> para);

public:
	string path_auth_inv_index_dir;
	string path_catg_inv_index_dir;
	string path_dict_dir;
	string path_fwd_index_dir;
	string path_inv_index_dir;
	string path_semi_wiki_dir;
	int inverted_index_barrels;
	string m_mode;
	string m_zone;
};

#endif
