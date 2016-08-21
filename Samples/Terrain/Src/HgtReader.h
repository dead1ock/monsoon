/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <fstream>

/**
 * HGT file format reader.
 */
class HGTReader
{
public:
	HGTReader(const char* filename);
	HGTReader();
	~HGTReader();

	void Load(const char* filename);

public:
	float** mHeightMap;
	float mMaxHeight, mMinHeight;
	int mResolution;
	std::ifstream mFile;
};