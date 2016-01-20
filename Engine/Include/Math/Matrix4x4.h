/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_MATH_MATRIX4X4_
#define _MONSOON_MATH_MATRIX4X4_

#include <Platform\Types.h>
#include <Platform\DynLib.h>

#include "Vector.h"

namespace Monsoon {
	namespace Math {

		/**
		* A 4x4 matrix implementation in row vector form.
		*/
		class DYNLIB Matrix4x4
		{
		public:
			/*
			 * Identity matrix constructor.
			 */
			Matrix4x4();
			Matrix4x4(float matrix[4][4]);
			~Matrix4x4();

			void operator*=(Matrix4x4& other); // Matrix Multiplication
			Matrix4x4 operator*(Matrix4x4& other);

			Matrix4x4 operator*(const Vector4& other); // Matrix - Vector (3x1 Matrix) Multiplication

			bool operator==(Matrix4x4& other); // Matrix Equality

			void Transpose();
			Matrix4x4 GetTranspose();

			Math::Vector4 GetRow(U8 row);
			Math::Vector4 GetColumn(U8 col);

			void SetRow(U8 row, Vector4 data);
			void SetColumn(U8 col, Vector4 data);

			float& operator()(U8 i, U8 j);

			static Matrix4x4 TransformMatrix(Vector3 position, Vector3 rotation, Vector3 scale);

		protected:
			float m[4][4];
		};

	}
}

#endif