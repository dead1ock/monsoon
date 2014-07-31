/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_PACKED_ARRAY_H_
#define _MONSOON_PACKED_ARRAY_H_

#include <vector>

#define DEFAULT_MAX_OBJECTS 4096

namespace Monsoon {
	namespace Util {

		/**
		* Holds a contiguous pool of objects which can be packed (sorted) for better cache performance when iterating, 
		* but accessed via an unchanging unique id.
		*/
		template<typename Id, typename Object>
		class PackedArray
		{
		public:
			PackedArray() {
				mPackedObjects.reserve(DEFAULT_MAX_OBJECTS);
				mIdTable.resize(DEFAULT_MAX_OBJECTS);
			}

			PackedArray(int maxSize) {
				mPackedObjects.reserve(maxSize);
				mIdTable.resize(maxSize);
			}

			inline void Add(Id index, Object& object) {
				mIdTable[index] = mPackedObjects.size();
				mPackedObjects.push_back(object);
			}

			inline Object& At(Id index) {
				return mPackedObjects[mIdTable[index]];
			}

			inline Object& operator[](Id index) {
				return mPackedObjects[mIdTable[index]];
			}

			inline void Remove(Id index) {
				int lastIndex = (mPackedObjects.size() - 1);

				// Swap and Pop.
				mPackedObjects[mIdTable[index]] = *(mPackedObjects.end() - 1);
				mPackedObjects.pop_back();

				// Update Id table with proper index.
				mIdTable[lastIndex] = mIdTable[index];
				mIdTable[index] = 0;
			}

			inline int Size() { return mPackedObjects.size(); }

		private:
			std::vector<Object> mPackedObjects;
			std::vector<Id> mIdTable;

		};

	}
}

#endif // _MONSOON_PACKED_ARRAY_H_