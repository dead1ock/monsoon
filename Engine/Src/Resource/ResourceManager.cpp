/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "Resource/ResourceManager.h"

#include <fstream>
#include <sstream>

using namespace Monsoon;
using namespace Monsoon::Resource;

ResourceManager::ResourceManager()
	: mNextResourceId(0)
{

}

ResourceManager::~ResourceManager()
{
	// Release all resource data left over.
	for (auto iter = mResources.begin(); iter != mResources.end(); iter++)
	{
		// TODO: Add log message here for unfreed resources.

		// Delete data.
		delete[] iter->second.data;
	}

	mResources.clear();
	mResourceIds.clear();
}

ResourceId ResourceManager::Load(std::string filename)
{
	ResourceId ret = Find(filename);
	if (ret == -1)
	{
		ResourceData newResourceData;
		
		LoadFile(filename, newResourceData);
		ret = mNextResourceId++;

		mResources.insert(std::pair<ResourceId, ResourceData>(ret, newResourceData));
	}
	

	return ret;
}

ResourceId ResourceManager::Find(std::string filename)
{
	auto iter = mResourceIds.find(filename);
	if (iter != mResourceIds.end())
		return iter->second;
	else
		return -1;
}

void ResourceManager::Unload(ResourceId id)
{
	auto resourceIter = mResources.find(id);
	if (resourceIter != mResources.end())
	{

		// Delete data.
		delete[] resourceIter->second.data;

		// Remove resource id from lookup table.
		auto idIter = mResourceIds.find(resourceIter->second.filename);
		if (idIter != mResourceIds.end())
			mResourceIds.erase(idIter);

		// Remove from resources table.
		mResources.erase(resourceIter);
	}
}

const ResourceData& ResourceManager::GetResourceData(ResourceId id)
{
	auto resourceIter = mResources.find(id);
	if (resourceIter != mResources.end())
		return resourceIter->second;
	else
		return ResourceData();
}

void ResourceManager::LoadFile(std::string filename, ResourceData& dest)
{
	std::ifstream iFile(filename, std::ios::binary);
	if (iFile.is_open())
	{
		// Get file size.
		iFile.seekg(0, std::ios::end);
		size_t fileSize = iFile.tellg();

		// Allocate space in memory.
		char* fileData = new char[fileSize];

		// Read into memory.
		iFile.seekg(0, std::ios::beg);
		iFile.read(fileData, fileSize);

		dest.filename = filename;
		dest.data = fileData;
		dest.size = fileSize;
		iFile.close();
	}
	else
	{
		dest.filename = "null";
		dest.data = 0;
		dest.size = 0;
	}
}