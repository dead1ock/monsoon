/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_MATH_MATRIX4X4_
#define _MONSOON_MATH_MATRIX4X4_

#include <Platform\Types.h>
#include <Platform\DynLib.h>

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

			bool operator==(Matrix4x4& other); // Matrix Equality

			void Transpose();
			Matrix4x4 GetTranspose();

			float* GetRow(U8 i);

			float& operator()(U8 i, U8 j);

		protected:
			float m[4][4];
		};

	}
}

#endif