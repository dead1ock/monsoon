/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_VECTOR_H_
#define _MONSOON_VECTOR_H_

#include <Platform/Export.h>
#include <Platform/Types.h>

namespace Monsoon {
	namespace Math {

		class Vector2;
		class Vector3;
		class Vector4;

		class DYNLIB Vector2
		{
		public:
			Vector2(float x, float y);
			Vector2(const Vector3& vector);
			~Vector2();

			Float Magnitude();
			Float Dot(Vector2& other);

			Vector2 Proj(Vector2& other);
			Vector2 Perp(Vector2& other);
			Vector2 Unit();

			Vector2 operator+(Vector2& other); // Vector Addition
			void operator+=(Vector2& other);

			Vector2 operator-(Vector2& other); // Vector Subtration
			void operator-=(Vector2& other);

			Float operator*(Vector2& other); // Dot

			Vector2 operator*(Float scalar); // Scalar Multiplication
			void operator*=(Float scalar);

			Vector2 operator/(Float scalar); // Scalar Division
			void operator/=(Float scalar);

			// These define the basis vectors for local space.
			static const Vector2 Right() { return Vector2(1.0f, 0.0f); }
			static const Vector2 Up() { return Vector2(0.0f, 1.0f); }
			static const Vector2 Left() { return Vector2(-1.0f, 0.0f); }
			static const Vector2 Down() { return Vector2(0.0f, -1.0f); }


			Float mX, mY;
		};

		class DYNLIB Vector3
		{
		public:
			Vector3(float x, float y, float z);
			Vector3(const Vector4& vec);
			~Vector3();

			Float Magnitude();
			Vector3 Cross(Vector3& other);

			Vector3 Proj(Vector3& other);
			Vector3 Perp(Vector3& other);
			Vector3 Unit();

			Float Dot(Vector3& other);

			Vector3 operator+(Vector3& other); // Vector Addition
			void operator+=(Vector3& other);

			Vector3 operator-(Vector3& other); // Vector Subtration
			void operator-=(Vector3& other);

			Vector3 operator*(Float scalar); // Scalar Multiplication
			void operator*=(Float scalar);

			Float operator*(Vector3& other); // Dot

			Vector3 operator/(Float scalar); // Scalar Division
			void operator/=(Float scalar);

			// These define the basis vectors for local space.
			static const Vector3 Front() { return Vector3(0.0f, 0.0f, 1.0f); }
			static const Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
			static const Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
			static const Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
			static const Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
			static const Vector3 Back() { return Vector3(0.0f, 0.0f, -1.0f); }

			Float mX, mY, mZ;
		};

		class DYNLIB Vector4
		{
		public:
			Vector4(float x, float y, float z, float w);
			Vector4(const Vector3& vec3, float w);
			~Vector4();

			Float Magnitude();

			Vector4 Proj(Vector4& other);
			Vector4 Perp(Vector4& other);
			Vector4 Unit();

			Float Dot(Vector4& other);

			Vector4 operator+(Vector4& other); // Vector Addition
			void operator+=(Vector4& other);

			Vector4 operator-(Vector4& other); // Vector Subtration
			void operator-=(Vector4& other);

			Vector4 operator*(Float scalar); // Scalar Multiplication
			void operator*=(Float scalar);

			Vector4 operator/(Float scalar); // Scalar Division
			void operator/=(Float scalar);

			Float mX, mY, mZ, mW;
		};
	}
}

#endif // _MONSOON_VECTOR_H_