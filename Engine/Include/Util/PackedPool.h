/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_PACKED_POOL_H_
#define _MONSOON_PACKED_POOL_H_

#include <vector>
#include <functional>

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
				 
				// If we are not the end of the vector, swap.
				// Copy the last element of the array over the one we want to remove, then pop the back.
				if (removeIndex != mPackedObjects.size()) {
					removeObject = *(mPackedObjects.end() - 1);
					mIndexTable[removeObject.second] = removeIndex;
				}

				mIndexTable[id] = USHRT_MAX;
				mPackedObjects.pop_back();
			}

			inline int Size() { return mPackedObjects.size(); }

			inline bool Exists(Id id) {
				return (mIndexTable[id] != USHRT_MAX);
			}

			inline Id IndexToId(Index index) {
				return (mPackedObjects[index].second);
			}

			inline void Sort(std::function<bool (const Object&, const Object&)> func)
			{
				// Sort packed objects.
				std::sort(mPackedObjects.begin(), mPackedObjects.end(), [&func](std::pair<Object, Id> a, std::pair <Object, Id> b) -> bool {
					return func(a.first, b.first);
				});

				// Rebuild index table.
				int x = 0;
				std::for_each(mPackedObjects.begin(), mPackedObjects.end(), [&x, this](const std::pair<Object, Id>& item) {
					mIndexTable[item.second] = x++;
				});
			}

		private:
			std::vector<std::pair<Object, Id>> mPackedObjects;
			std::vector<Index> mIndexTable;
		};

	}
}

#endif // _MONSOON_PACKED_POOL_H_