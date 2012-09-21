#ifndef PARAMETERS
#define PARAMETERS

#include <iostream>
#include <fstream>		
#include <string>
#include <vector>

using namespace std;

class parameters
{
public:
	parameters();
	parameters(const char* filePath);
	
private:
	string trim(const string& data);
	void disp(vector<string> para);

public:
	string path_input_dir;
	string path_index_dir;
	string path_dict_dir;
	int inverted_index_barrels;
};

#endif