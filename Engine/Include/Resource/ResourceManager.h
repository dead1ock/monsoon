/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_RESOURCEMANAGER_H_
/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#define _MONSOON_RESOURCEMANAGER_H_

#include <Platform/Types.h>
#include "Platform\DynLib.h"

#include <unordered_map>

namespace Monsoon
{
	namespace Resource
	{
		typedef U32 ResourceId;

		/**
		 * Resource record.
		 */
		struct ResourceData
		{
			std::string filename;
			char* data;
			U32 size;
		};

		/**
		 * Handles loading and unloading of resources from disk into memory, and is responsibile
		 * for providing the raw data of loaded resources.
		 */
		class DYNLIB ResourceManager
		{
		public:
			ResourceManager();
			~ResourceManager();

			ResourceId Load(std::string filename);
			ResourceId Find(std::string filename);
			void Unload(ResourceId resource);
			void Reload(ResourceId resource);

			const ResourceData& GetResourceData(ResourceId resource);

		private:

			void LoadFile(std::string filename, ResourceData& dest);

			std::unordered_map<ResourceId, ResourceData> mResources;
			std::unordered_map<std::string, ResourceId> mResourceIds;
			ResourceId mNextResourceId;
		};
	}
}

#endif // _MONSOON_RESOURCEMANAGER_H_