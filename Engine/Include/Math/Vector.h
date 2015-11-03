/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_VECTOR_H_
#define _MONSOON_VECTOR_H_

namespace Monsoon {
	namespace Math {

		class Vector2;
		class Vector3;

		class Vector2
		{
		public:
			Vector2(float x, float y);
			~Vector2();

			float Magnitude();
			float Dot(Vector2& other);

			Vector2 operator+(Vector2& other); // Vector Addition
		protected:
			float mX, mY;
		};

		class Vector3
		{
		public:
			Vector3(float x, float y, float z);
			~Vector3();

			float Magnitude();
			Vector3 Cross(Vector3& other);
			float Dot(Vector3& other);

			Vector3 operator+(Vector3& other); // Vector Addition
		protected:
			float mX, mY, mZ;
		};
	}
}

#endif // _MONSOON_VECTOR_H_