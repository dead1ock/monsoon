/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_PACKED_POOL_H_
#define _MONSOON_PACKED_POOL_H_

#include <vector>

#define DEFAULT_MAX_POOL_SIZE 4096

namespace Monsoon {
	namespace Util {

		/**
		* Holds a contiguous pool of objects which can be packed (sorted) for better performance when iterating, 
		* but accessed via an unchanging unique identifier (for example, a handle or entity id). The goal of this 
		* container is to pack objects together as close as possible in memory to increase cache coherency when iterating 
		* over the pool. This comes at a cost of some memory overhead, due to id-to-index bookkeeping.
		*/
		template<typename Id, typename Object>
		class PackedPool
		{
		private:
			typedef U16 Index;

		public:
			PackedPool() {
				mPackedObjects.reserve(DEFAULT_MAX_POOL_SIZE);
				mIndexTable.resize(DEFAULT_MAX_POOL_SIZE, USHRT_MAX);
			}

			PackedPool(U16 maxSize) {
				mPackedObjects.reserve(maxSize);
				mIndexTable.resize(maxSize, USHRT_MAX);
			}

			inline void Add(Id id, Object& object) {
				mIndexTable[id] = mPackedObjects.size();
				mPackedObjects.push_back(std::pair<Object, Id>(object, id));
			}

			/**
			 * Returns the object from the packed array by INDEX.
			 */
			inline Object& At(Index index) {
				return mPackedObjects[index].first;
			}

			/**
			* Returns the object from the packed array by ID.
			*/
			inline Object& operator[](Id id) {
				return mPackedObjects[mIndexTable[id]].first;
			}

			inline void Remove(Id id) {
				Index removeIndex = mIndexTable[id];
				std::pair<Object, Id>& removeObject = mPackedObjects[removeIndex];
				 
				// Swap and pop.
				// Copy the last element of the array over the one we want to remove, then pop the back.
				removeObject = *(mPackedObjects.end() - 1);
				mPackedObjects.pop_back();

				// Update indices.
				mIndexTable[id] = USHRT_MAX;
				mIndexTable[removeObject.second] = removeIndex;
			}

			inline int Size() { return mPackedObjects.size(); }

		private:
			std::vector<std::pair<Object, Id>> mPackedObjects;
			std::vector<Index> mIndexTable;
		};

	}
}

#endif // _MONSOON_PACKED_POOL_H_