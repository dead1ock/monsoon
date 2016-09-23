/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_AABB_H_
#define _MONSOON_AABB_H_

#include <Platform/Export.h>

namespace Monsoon {
	namespace Math {

		/**
		 * An Axis-Aligned Bounding Box
		 */
		class DYNLIB AABB
		{
		public:

			/**
			 * Default constructor.
			 */
			AABB();

			/**
			 * 2d AABB constructor.
			 * @param x x-coordinate of the center of the box.
			 * @param y y-coordinate of the center of the box.
			 * @param width width of the bounding box.
			 * @param height height of the bounding box.
			 */
			AABB(float x, float y, float width, float height);

			/**
			 * 3d AABB constructor.
			 * @param x x-coordinate of the center of the box.
			 * @param y y-coordinate of the center of the box.
			 * @param z z-coordinate of the center of the box.
			 * @param width width of the bounding box.
			 * @param height height of the bounding box.
			 * @param depth depth of the bounding box.
			 */
			AABB(float x, float y, float z, float width, float height, float depth);

			/**
			 * Default destructor.
			 */
			~AABB();

			/**
			 * Checks for an intersection between two AABBs (this and other).
			 * @param other The other bounding box.
			 * @return True if there is an intersection, otherwise false.
			 */
			bool Intersects(AABB& other);

			/**
			 * Sets the position of the bounding box.
			 * @param x x-coordinate of the center of the box.
			 * @param y y-coordinate of the center of the box.
			 * @param z z-coordinate of the center of the box. (Default = 0.0f)
			 */
			void SetPosition(float x, float y, float z = 0.0f);

			/**
			* Sets the scale of the bounding box.
			* @param x x scale of the box.
			* @param y y scale of the box.
			* @param z z scale of the box. (Default = 0.0f)
			*/
			void SetScale(float x, float y, float z = 0.0f);

		private:
			float mWidth, mHeight, mDepth;
			float mX, mY, mZ;
		};

	}
}

#endif // _MONSOON_AABB_H_