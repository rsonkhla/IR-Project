/*
 * Reader.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashu1710
 */

#ifndef Reader_H_
#define Reader_H_

#include <fstream>
#include <vector>
#include "posting.h"
#include <cstdlib>

class IndexReader
{
	public:
		Posting readLinesFromFile(ifstream& ifile, string filename);
		void resetStream(ifstream& ifile);
		IndexReader();
};

#endif /* Reader_H_ */
