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
	enum TYPE {
		NOT_LOADED,
		THREE_ARC_SECS,
		ONE_ARC_SECS
	};

	HGTReader(const char* filename);
	HGTReader();
	~HGTReader();

	void Load(const char* filename);
	TYPE GetType() {
		if (mResolution == 1201)
			return THREE_ARC_SECS;
		else if (mResolution == 3601)
			return ONE_ARC_SECS;
		else
			return NOT_LOADED;
	}

	unsigned int GetResolution() {
		return mResolution;
	}

public:
	float** mHeightMap;
	float mMaxHeight, mMinHeight;
	unsigned int mResolution;
	TYPE mType;
	std::ifstream mFile;
};