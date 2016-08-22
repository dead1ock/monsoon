/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "HgtReader.h"

#include <sys/stat.h>

long GetFileSize(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

HGTReader::HGTReader()
	: mMaxHeight(0.0f)
	, mMinHeight(50000.0f)
	, mResolution(0) {

}

HGTReader::HGTReader(const char* filename)
	: mFile(filename, std::ios::binary)
	, mMaxHeight(0.0f)
	, mMinHeight(50000.0f)
	, mResolution(0) {
	Load(filename);
}

void HGTReader::Load(const char* filename)
{
	long size = GetFileSize(filename);

	if (!mFile.is_open())
		mFile.open(filename, std::ios::binary);

	if (size == std::streampos(2884802)) // 1201 Resolution (3 arc-seconds)
		mResolution = 1201;
	else if (size == std::streampos(25934402)) // 3067 Resolution (1 arc-second)
		mResolution = 3601;
	else {
		throw std::exception("File format is not recognized.");
	}

	mHeightMap = new float*[mResolution];
	for (int i = 0; i < mResolution; i++)
		mHeightMap[i] = new float[mResolution];

	for (int x = 0; x < mResolution; x++)
	{
		for (int y = 0; y < mResolution; y++)
		{
			float elevation = ((int)mFile.get() * 256) + mFile.get();
			mHeightMap[x][y] = elevation;

			if (elevation > mMaxHeight)
				mMaxHeight = elevation;
			if (elevation < mMinHeight)
				mMinHeight = elevation;
		}
	}
}

HGTReader::~HGTReader() {
	if (!mFile.is_open())
		mFile.close();
}