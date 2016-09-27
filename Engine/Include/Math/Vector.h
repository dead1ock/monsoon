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
			/**
			 * Constructs Vector2 from x, y coordinates.
			 *
			 * @param x x-coordinate of the vector head.
			 * @param y y-coordinate of the vector head.
			 */
			Vector2(float x, float y);

			/**
			 * Constructs Vector2 from Vector3. (z = 0)
			 *
			 * @param vector The 3 dimensional vector to be expressed in 2 dimensions.
			 */
			Vector2(const Vector3& vector);

			/**
			 * Default Destructor
			 */
			~Vector2();

			/**
			 * Calculates the magnitude of the vector. Geometrically
			 * this is the "length" of the vector from the origin to 
			 * the head.
			 * 
			 * @return The magnitude of this vector.
			 */
			Float Magnitude();

			/**
			 * The dot product (this * other).
			 *
			 * @return The result of applying the dot operation of this on the other vector.
			 */
			Float Dot(Vector2& other);

			/**
			 * The projection of this onto a different vector.
			 *
			 * @param other The projection ("shadow") of this onto the other vector.
			 */
			Vector2 Proj(Vector2& other);

			/**
			 * The vector perpendicular this and the projection
			 * of this onto a different vector.
			 *
			 * @param Vector2 The vector we are projecting onto.
			 * @return A vector perpendicular to this and the projection of this onto other.
			 */
			Vector2 Perp(Vector2& other);

			/**
			 * Builds a unit vector of from this vector.
			 *
			 * @return This vector in unit vector form.
			 */
			Vector2 Unit();

			/**
			 * Adds components of this and another vector.
			 * 
			 * @param other The other vector.
			 * @return The result of adding vectors component-wise.
			 */
			Vector2 operator+(Vector2& other); // Vector Addition

			/**
			* Adds components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator+=(Vector2& other);

			/**
			* Subtracts components of this and another vector.
			*
			* @param other The other vector.
			* @return The result of subtracting vectors component-wise.
			*/
			Vector2 operator-(Vector2& other);

			/**
			* Subtracts components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator-=(Vector2& other);

			/**
			 * Dot product. See Float Dot(Vector2& other).
			 */
			Float operator*(Vector2& other); // Dot

			/**
			 * Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar.
			 *
			 * @param scalar The scalar to multiply each component by.
			 * @return The result of multiplying this vector's components by the scalar.
			 */
			Vector2 operator*(Float scalar);

			/**
			* Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar,
			* storing the result in this vector.
			*
			* @param scalar The scalar to multiply each component by.
			*/
			void operator*=(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar.
			*
			* @param scalar The scalar to divide each component by.
			* @return The result of dividing this vector's components by the scalar.
			*/
			Vector2 operator/(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar,
			* storing the results in this vector.
			*
			* @param scalar The scalar to divide each component by.
			*/
			void operator/=(Float scalar);

			/**
			 * Creates a vector which points right in 2D GLOBAL space.
			 *
			 * @return A vector which points right in 2D GLOBAL space.
			 */
			static const Vector2 Right() { return Vector2(1.0f, 0.0f); }

			/**
			* Creates a vector which points up in 2D GLOBAL space.
			*
			* @return A vector which points up in 2D GLOBAL space.
			*/
			static const Vector2 Up() { return Vector2(0.0f, 1.0f); }

			/**
			* Creates a vector which points left in 2D GLOBAL space.
			*
			* @return A vector which points left in 2D GLOBAL space.
			*/
			static const Vector2 Left() { return Vector2(-1.0f, 0.0f); }

			/**
			* Creates a vector which points down in 2D GLOBAL space.
			*
			* @return A vector which points down in 2D GLOBAL space.
			*/
			static const Vector2 Down() { return Vector2(0.0f, -1.0f); }


			Float mX, mY;
		};

		class DYNLIB Vector3
		{
		public:

			/**
			 * Constructs Vector3 from x, y, z coordinates.
			 *
			 * @param x The x-coordinate of the vector head.
			 * @param y The y-coordinate of the vector head.
			 * @param z The z-coordinate of the vector head.
			 */
			Vector3(float x, float y, float z);

			/**
			 * Constructs Vector3 from a Vector4 (w = 0).
			 * 
			 * @param vec The Vector4 to be represented in 3 dimensions.
			 */
			Vector3(const Vector4& vec);

			/**
			 * Default destructor.
			 */
			~Vector3();

			/**
			* Calculates the magnitude of the vector. Geometrically
			* this is the "length" of the vector from the origin to
			* the head.
			*
			* @return The magnitude of this vector.
			*/
			Float Magnitude();

			/**
			 * Calculates the vector perpendicular to this and another vector.
			 *
			 * @param other The other vector to "cross" with this vector.
			 * @returns The vector perpendicular to this and other.
			 */
			Vector3 Cross(Vector3& other);

			/**
			* The projection of this onto a different vector.
			*
			* @param other The projection ("shadow") of this onto the other vector.
			*/
			Vector3 Proj(Vector3& other);

			/**
			* The vector perpendicular this and the projection
			* of this onto a different vector.
			*
			* @param Vector2 The vector we are projecting onto.
			* @return A vector perpendicular to this and the projection of this onto other.
			*/
			Vector3 Perp(Vector3& other);

			/**
			* Builds a unit vector of from this vector.
			*
			* @return This vector in unit vector form.
			*/
			Vector3 Unit();

			/**
			* The dot product (this * other).
			*
			* @return The result of applying the dot operation of this on the other vector.
			*/
			Float Dot(Vector3& other);

			/**
			* Adds components of this and another vector.
			*
			* @param other The other vector.
			* @return The result of adding vectors component-wise.
			*/
			Vector3 operator+(Vector3& other); // Vector Addition

			/**
			* Adds components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator+=(Vector3& other);

			/**
			* Subtracts components of this and another vector.
			*
			* @param other The other vector.
			* @return The result of subtracting vectors component-wise.
			*/
			Vector3 operator-(Vector3& other);

			/**
			* Subtracts components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator-=(Vector3& other);

			/**
			* Dot product. See Float Dot(Vector2& other).
			*/
			Float operator*(Vector3& other); // Dot

			/**
			* Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar.
			*
			* @param scalar The scalar to multiply each component by.
			* @return The result of multiplying this vector's components by the scalar.
			*/
			Vector3 operator*(Float scalar);

			/**
			* Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar,
			* storing the result in this vector.
			*
			* @param scalar The scalar to multiply each component by.
			*/
			void operator*=(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar.
			*
			* @param scalar The scalar to divide each component by.
			* @return The result of dividing this vector's components by the scalar.
			*/
			Vector3 operator/(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar,
			* storing the results in this vector.
			*
			* @param scalar The scalar to divide each component by.
			*/
			void operator/=(Float scalar);

			/**
			* Creates a vector which points forward in 3D GLOBAL space.
			*
			* @return A vector which points forward in 3D GLOBAL space.
			*/
			static const Vector3 Front() { return Vector3(0.0f, 0.0f, 1.0f); }

			/**
			* Creates a vector which points right in 3D GLOBAL space.
			*
			* @return A vector which points right in 3D GLOBAL space.
			*/
			static const Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }

			/**
			* Creates a vector which points up in 3D GLOBAL space.
			*
			* @return A vector which points up in 3D GLOBAL space.
			*/
			static const Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }

			/**
			* Creates a vector which points left in 3D GLOBAL space.
			*
			* @return A vector which points left in 3D GLOBAL space.
			*/
			static const Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }

			/**
			* Creates a vector which points down in 3D GLOBAL space.
			*
			* @return A vector which points down in 3D GLOBAL space.
			*/
			static const Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }

			/**
			* Creates a vector which points back in 3D GLOBAL space.
			*
			* @return A vector which points back in 3D GLOBAL space.
			*/
			static const Vector3 Back() { return Vector3(0.0f, 0.0f, -1.0f); }

			Float mX, mY, mZ;
		};

		class DYNLIB Vector4
		{
		public:
			/**
			* Constructs Vector4 from x, y, z, w coordinates.
			*
			* @param x The x-coordinate of the vector head.
			* @param y The y-coordinate of the vector head.
			* @param z The z-coordinate of the vector head.
			* @param w The w-coordinate of the vector head.
			*/
			Vector4(float x, float y, float z, float w);

			/**
			* Constructs Vector4 from a Vector3.
			*
			* @param vec The Vector4 to be represented in 4 dimensions.
			* @param w The w-component of the new vector.
			*/
			Vector4(const Vector3& vec3, float w);

			/**
			 * Default destructor.
			 */
			~Vector4();

			/**
			* Calculates the magnitude of the vector. Geometrically
			* this is the "length" of the vector from the origin to
			* the head.
			*
			* @return The magnitude of this vector.
			*/
			Float Magnitude();

			/**
			* The projection of this onto a different vector.
			*
			* @param other The projection ("shadow") of this onto the other vector.
			*/
			Vector4 Proj(Vector4& other);
			
			/**
			* The vector perpendicular this and the projection
			* of this onto a different vector.
			*
			* @param other The vector we are projecting onto.
			* @return A vector perpendicular to this and the projection of this onto other.
			*/
			Vector4 Perp(Vector4& other);

			/**
			* Builds a unit vector of from this vector. (Magnitude of 1)
			*
			* @return This vector in unit vector form.
			*/
			Vector4 Unit();

			/**
			* The dot product (this * other).
			*
			* @return The result of applying the dot operation of this on the other vector.
			*/
			Float Dot(Vector4& other);

			/**
			* Adds components of this and another vector.
			*
			* @param other The other vector.
			* @return The result of adding vectors component-wise.
			*/
			Vector4 operator+(Vector4& other); // Vector Addition

			/**
			* Adds components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator+=(Vector4& other);

			/**
			* Subtracts components of this and another vector.
			*
			* @param other The other vector.
			* @return The result of subtracting vectors component-wise.
			*/
			Vector4 operator-(Vector4& other);

			/**
			* Subtracts components of this and another vector, storing
			* the results in this vector.
			*
			* @param other The other vector.
			*/
			void operator-=(Vector4& other);

			/**
			* Dot product. See Float Dot(Vector2& other).
			*/
			Float operator*(Vector4& other); // Dot

			/**
			* Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar.
			*
			* @param scalar The scalar to multiply each component by.
			* @return The result of multiplying this vector's components by the scalar.
			*/
			Vector4 operator*(Float scalar);

			/**
			* Vector-Scalar multiplication. Multiplies each component (x, y) of the vector by the scalar,
			* storing the result in this vector.
			*
			* @param scalar The scalar to multiply each component by.
			*/
			void operator*=(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar.
			*
			* @param scalar The scalar to divide each component by.
			* @return The result of dividing this vector's components by the scalar.
			*/
			Vector3 operator/(Float scalar);

			/**
			* Vector-Scalar division. Divides each component (x, y) of the vector by the scalar,
			* storing the results in this vector.
			*
			* @param scalar The scalar to divide each component by.
			*/
			void operator/=(Float scalar);

			Float mX, mY, mZ, mW;
		};
	}
}

#endif // _MONSOON_VECTOR_H_