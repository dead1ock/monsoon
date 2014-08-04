/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_AABB_H_
#define _MONSOON_AABB_H_

#include <Platform/Export.h>

namespace Monsoon {
	namespace Math {

		class DYNLIB AABB
		{
		public:
			AABB();
			AABB(float x, float y, float width, float height);
			AABB(float x, float y, float z, float width, float height, float depth);
			~AABB();

			bool Intersects(AABB& other);

			float mWidth, mHeight, mDepth;
			float mX, mY, mZ;
		};

	}
}

#endif // _MONSOON_AABB_H_