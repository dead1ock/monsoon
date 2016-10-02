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
		* over the pool. This comes at a cost of significant memory overhead, due to id-to-index bookkeeping.
		*/
		template<typename Id, typename Object>
		class PackedPool
		{
		private:
			typedef U16 Index;

		public:
			/**
			 * Default constructor.
			 */
			PackedPool() {
				mPackedObjects.reserve(DEFAULT_MAX_POOL_SIZE);
				mIndexTable.resize(DEFAULT_MAX_POOL_SIZE, USHRT_MAX);
			}

			/**
			 * Constructs a packed pool with a maximum size.
			 *
			 * @param maxSize The maximum size of the pool.
			 */
			PackedPool(U16 maxSize) {
				mPackedObjects.reserve(maxSize);
				mIndexTable.resize(maxSize, USHRT_MAX);
			}

			/**
			 * Adds an object to the pool.
			 * Performane: O(1)
			 * 
			 * @param id The unique id of the object.
			 * @param object The object.
			 */
			inline void Add(Id id, Object& object) {
				mIndexTable[id] = (U16)mPackedObjects.size();
				mPackedObjects.push_back(std::pair<Object, Id>(object, id));
			}

			/**
			 * Returns the object from the packed array by INDEX. O(1)
			 */
			inline Object& At(Index index) {
				return mPackedObjects[index].first;
			}

			/**
			* Returns the object from the packed array by ID. O(1)
			*/
			inline Object& operator[](Id id) {
				return mPackedObjects[mIndexTable[id]].first;
			}

			/**
			 * Removes the object with the associated unique-id from the table.
			 * Performane: O(1)
			 *
			 * @param id The unique-id of the object to remove.
			 */
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

			/**
			 * @returns The number of objects in the packed pool.
			 */
			inline U16 Size() { return (U16) mPackedObjects.size(); }

			/**
			 * Queries the pool to see if the specified object
			 * exists in it.
			 * Performance O(1)
			 *
			 * @param id The id of the object.
			 * @returns True if the object exists in the pool, otherwise false.
			 */
			inline bool Exists(Id id) {
				return (mIndexTable[id] != USHRT_MAX);
			}

			/**
			 * Takes an index and returns the corresponding object id, if
			 * the index is valid (and object exists with that index).
			 * Performance: O(1)
			 *
			 * @param index - The index of the object you want to find the id of.
			 * @returns The id of the object at the specified index, if it exists.
			 */
			inline Id IndexToId(Index index) {
				if (index < 0 || index >(mPackedObjects.size() - 1))
					return 0; // Temporary fix. This will break anything which tries to create a 
								// packed pool with an Id type which is not some kind of primative number type.
				else
					return (mPackedObjects[index].second);
			}

			/**
			 * Sorts the packed objects and rebuilds the index table. 
			 * This is current a very expensive operation for large datasets! 
			 * Performance: O(n)
			 *
			 * @param A function pointer to the comparitor function which will determine which of 2
			 * elements will come first. This function should return TRUE if object 1 comes before object 2
			 * in total order. 
			 *
			 * Usually it is useful to pass a lambda as this parameter.
			 * 
			 * Example:
			 * 
			 * pool.Sort([](const Object& obj1, const Object& obj2) {
			 *    return (obj1 < obj2);
			 * }
			 */
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