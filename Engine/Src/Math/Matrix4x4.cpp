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

float* Matrix4x4::GetRow(U8 i)  {
	return m[i];
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