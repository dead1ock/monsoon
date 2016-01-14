/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_MATH_MATRIX4X4_
#define _MONSOON_MATH_MATRIX4X4_

#include <Platform\DynLib.h>

namespace Monsoon {
	namespace Math {

		/**
		* A 4x4 matrix implementation in row vector form.
		*/
		class DYNLIB Matrix4x4
		{
		public:
			Matrix4x4();
			~Matrix4x4();

			void operator*=(Matrix4x4& other); // Matrix Multiplication
			Matrix4x4 operator*(Matrix4x4& other);

			float m[4][4];
		};

	}
}

#endif