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
			 * Default constructor.
			 */
			Matrix4x4();

			/**
			 * Array to Matrix constructor.
			 */
			Matrix4x4(float matrix[4][4]);

			/**
			 * Default destructor.
			 */
			~Matrix4x4();

			/**
			 * Matrix multiplication.
			 *
			 * @return The matrix product [this][other].
			 */
			void operator*=(Matrix4x4& other); // Matrix Multiplication
			Matrix4x4 operator*(Matrix4x4& other);

			/**
			 * Matrix - Vector Multiplication
			 *
			 * @return Returns the product of Matrix*Vector4.
			 */
			Matrix4x4 operator*(const Vector4& other); // Matrix - Vector (3x1 Matrix) Multiplication

			/**
			 * Compares the cells of this matrix against a different matrix for equality. 
			 * Does NOT check for equivalence of transformations under different basis.
			 * 
			 * @return True if the 2 matrices are cell equivalents, otherwise false.
			 */
			bool operator==(Matrix4x4& other);

			/**
			 * Applies a Transpose operation on this matrix.
			 */
			void Transpose();

			/**
			 * Returns the transpose of this matrix. This does not effect the values
			 * contained in this matrix.
			 *
			 * @return The transpose of this matrix.
			 */
			Matrix4x4 GetTranspose();

			/**
			 * Extracts a row from matrix and returns it in vector form.
			 *
			 * @param row The row number to extract.
			 * @return The extracted row in Vector4 form.
			 */
			Math::Vector4 GetRow(U8 row);

			/**
			 * Extracts a column from the matrix and returns it in vector form.
			 *
			 * @param column The column number to extract.
			 * @return The extracted column in Vector4 form.
			 */
			Math::Vector4 GetColumn(U8 col);

			/**
			 * Sets a row to the provided vector.
			 *
			 * @param row The row number to set.
			 * @param Vector4 The data to set the row to.
			 */
			void SetRow(U8 row, Vector4 data);

			/**
			* Sets a column to the provided vector.
			*
			* @param row The column number to set.
			* @param Vector4 The data to set the row to.
			*/
			void SetColumn(U8 col, Vector4 data);

			/**
			 * Accesses a column and row cell.
			 *
			 * @param i The column to access.
			 * @param i The row to access.
			 * @return The value at that column and row.
			 */
			float operator()(U8 i, U8 j);

			/**
			 * Returns a transformation matrix which applies a translation, rotation, and/or scale
			 * operations.
			 *
			 * @param position The translation this matrix will perform.
			 * @param rotation The rotation this matrix will perform.
			 * @param scale The scaling operation this matrix will perform.
			 * @return The matrix which perform a translation-rotation-scale action.
			 */
			static Matrix4x4 TransformMatrix(Vector3 position, Vector3 rotation, Vector3 scale);

			/**
			 * Returns an identity matrix.
			 */
			static Matrix4x4 Identity();

		protected:
			float m[4][4];
			static Matrix4x4 mIdentityMatrix;
		};

	}
}

#endif