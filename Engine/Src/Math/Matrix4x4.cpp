/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "Math/Matrix4x4.h"

using namespace Monsoon;
using namespace Monsoon::Math;

Matrix4x4::Matrix4x4() {
	m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = 0;
	m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;	m[1][3] = 0;
	m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;	m[2][3] = 0;
	m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

Matrix4x4::Matrix4x4(float matrix[4][4]) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = matrix[i][j];
		}
	}
}

Matrix4x4::~Matrix4x4() {

}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& other) {
	Matrix4x4 m0; // Result
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m0.m[i][j] = (m[i][0] * other.m[0][j]) + (m[i][1] * other.m[1][j]) + (m[i][2] * other.m[2][j]) + (m[i][3] * other.m[3][j]);
		}
	}
	return m0;
}

void Matrix4x4::operator*=(Matrix4x4& other) {

}

Matrix4x4 Matrix4x4::operator*(const Vector4& other) {
	Matrix4x4 vecMat;
	vecMat.SetRow(0, other);
	vecMat.SetRow(1, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	vecMat.SetRow(2, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	vecMat.SetRow(3, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	return vecMat * (*this);
}

bool Matrix4x4::operator==(Matrix4x4& other) {
	bool equal = true;
	for (int i = 0; i < 4; i++)
	{
		if (equal == false)
			break;

		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != other.m[i][j]) {
				equal = false;
				break;
			}
		}
	}

	return equal;
}

float& Matrix4x4::operator()(U8 i, U8 j)  {
	return m[i][j];
}

Vector4 Matrix4x4::GetRow(U8 row)
{
	return Vector4(m[row][0], m[row][1], m[row][2], m[row][3]);
}

Vector4 Matrix4x4::GetColumn(U8 col)
{
	return Vector4(m[0][col], m[1][col], m[2][col], m[3][col]);
}

void Matrix4x4::SetRow(U8 row, Vector4 data)
{
	m[row][0] = data.mX;
	m[row][1] = data.mY;
	m[row][2] = data.mZ;
	m[row][3] = data.mW;
}

void Matrix4x4::SetColumn(U8 col, Vector4 data)
{
	m[0][col] = data.mX;
	m[1][col] = data.mY;
	m[2][col] = data.mZ;
	m[3][col] = data.mW;
}

Matrix4x4 Matrix4x4::TransformMatrix(Vector3 position, Vector3 rotation, Vector3 scale)
{
	Matrix4x4 rotX, rotY, rotZ;
	float yaw = rotation.mY;
	float pitch = rotation.mX;
	float roll = rotation.mZ;

	rotZ.SetRow(0, Vector4(cos(roll), sin(roll), 0.0f, 0.0f));
	rotZ.SetRow(1, Vector4(-sin(roll), cos(roll), 0.0f, 0.0f));
	rotZ.SetRow(2, Vector4(0.0f, 0.0f, 1.0f, 0.0f));
	rotZ.SetRow(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	rotY.SetRow(0, Vector4(cos(yaw), 0.0f, -sin(yaw), 0.0f));
	rotY.SetRow(1, Vector4(0.0f, 1.0f, 0.0f, 0.0f));
	rotY.SetRow(2, Vector4(sin(yaw), 0.0f, -cos(yaw), 0.0f));
	rotY.SetRow(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	rotX.SetRow(0, Vector4(1.0f, 0.0f, 0.0f, 0.0f));
	rotX.SetRow(1, Vector4(0.0f, cos(pitch), sin(pitch), 0.0f));
	rotX.SetRow(2, Vector4(0.0f, -sin(pitch), cos(pitch), 0.0f));
	rotX.SetRow(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	Matrix4x4 result = rotX * rotY * rotZ;
	result.SetRow(3, Vector4(position, 1.0f));

	return result;
}

void Matrix4x4::Transpose() {
	float trans[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			trans[i][j] = m[j][i];
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = trans[i][j];
		}
	}
}

Matrix4x4 Matrix4x4::GetTranspose() {
	float trans[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			trans[i][j] = m[j][i];
		}
	}

	return Matrix4x4(trans);
}