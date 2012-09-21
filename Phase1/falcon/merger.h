/*
 * merger.h
 *
 *  Created on: Oct 28, 2011
 *      Author: ashu1710
 */

#ifndef MERGER_H_
#define MERGER_H_

#include "CommonSettingsAndUtilities.h"

#ifdef MY_WINDOWS
	//#include <Windows.h>
#else 
	#include <dirent.h>
#endif

#include <string>
#include <sys/types.h>
#include <errno.h>
#include <queue>
#include <map>
#include "reader.h"
#include "posting.h"

class Merger
{
	private:
		string indexInputDirectory;
		string indexOutputDirectory;

		void getdir(string dir, vector<string> &files);

		void getStringAfterTerm(const char* str);

	public:

		void mergeIndex(int indexNumber);
		Merger(const char* indir, const char* outdir);
		virtual ~Merger();
};

#endif /* MERGER_H_ */
