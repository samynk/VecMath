#include "IMatrix.h"
#include <iostream>
#include "Scalar.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Quaternion.h"

IMatrix::IMatrix(Type type, AddressMode mode, int rows, int columns)
	:IMatrix(type, mode, 0, rows, columns)
{
}

IMatrix::IMatrix(Type type, AddressMode mode, float borderValue, int rows, int columns)
	: m_AddressMode(mode), m_Type(type), m_BorderValue(borderValue), m_Rows(rows), m_Columns(columns)
{
}

void IMatrix::print(HANDLE console)
{
	SetConsoleTextAttribute(console, 8);
	if (m_Rows > 1) {
		std::cout << "{";
	}
	for (int r = 0; r < m_Rows; ++r) {
		std::cout << "[";
		for (int c = 0; c < m_Columns; ++c) {
			SetConsoleTextAttribute(console, 10);
			std::cout << get(r, c);
			if (c + 1 < m_Columns) {
				SetConsoleTextAttribute(console, 8);
				std::cout << ",";
			}
		}
		if (r + 1 < m_Rows) {
			SetConsoleTextAttribute(console, 8);
			std::cout << ",";
		}
		SetConsoleTextAttribute(console, 8);
		std::cout << "]";
	}
	if (m_Rows > 1) {
		SetConsoleTextAttribute(console, 8);
		std::cout << "}";
	}
}

float IMatrix::magnitude()
{
	float sum = 0;
	for (int r = 0; r < m_Rows; ++r) {
		for (int c = 0; c < m_Columns; ++c) {
			float value = get(r, c);
			sum += (value*value);
		}
	}
	return sqrt(sum);
}

IMatrix* IMatrix::conjugate()
{
	return new Scalar(NAN);
}

IMatrix* IMatrix::inverse()
{
	IMatrix* result = maxMatrix(this, nullptr);
	IMatrix::unaryOp(this, [](float x) {return 1/x; }, result);
	return result;

}

IMatrix* IMatrix::maxMatrix(IMatrix* op1, IMatrix* op2)
{
	int rows = std::max(
		op1 != nullptr ? op1->m_Rows : 0,
		op2 != nullptr ? op2->m_Rows : 0
	);
	int columns = std::max(
		op1 != nullptr ? op1->m_Columns : 0,
		op2 != nullptr ? op2->m_Columns : 0
	);
	if (rows == 1) {
		switch (columns) {
		case 1:return new Scalar();
		case 2:return new Vector2D();
		case 3:return new Vector3D();
		case 4:return new Quaternion();
		}
	}
	else {
		return new Scalar(NAN);
	}
}

IMatrix* IMatrix::maxRowMatrix(IMatrix* op1, IMatrix* op2)
{
	int rows = std::max(
		op1 != nullptr ? op1->m_Rows : 0,
		op2 != nullptr ? op2->m_Rows : 0
	);
	int columns = 1;
	if (rows == 1) {
		switch (columns) {
		case 1:return new Scalar();
		case 2:return new Vector2D();
		case 3:return new Vector3D();
		case 4:return new Quaternion();
		}
	}
	else {
		return new Scalar(NAN);
	}
}

IMatrix* IMatrix::maxColumnMatrix(IMatrix* op1, IMatrix* op2)
{
	int rows = 1;
	int columns = std::max(
		op1 != nullptr ? op1->m_Columns : 0,
		op2 != nullptr ? op2->m_Columns : 0
	);
	if (rows == 1) {
		switch (columns) {
		case 1:return new Scalar();
		case 2:return new Vector2D();
		case 3:return new Vector3D();
		case 4:return new Quaternion();
		}
	}
	else {
		return new Scalar(NAN);
	}
}

void IMatrix::binaryOp(IMatrix* op1,
	IMatrix* op2, std::function<float(float, float)> func, IMatrix* result)
{
	for (int r = 0; r < result->m_Rows; ++r) {
		for (int c = 0; c < result->m_Columns; ++c) {
			float x1 = op1->get(r, c);
			float x2 = op2->get(r, c);
			float fr = func(x1, x2);
			result->set(r, c, fr);
		}
	}
}

void IMatrix::unaryOp(IMatrix* op1, std::function<float(float)> func, IMatrix* result)
{
	for (int r = 0; r < result->m_Rows; ++r) {
		for (int c = 0; c < result->m_Columns; ++c) {
			float x1 = op1->get(r, c);
			float fr = func(x1);
			result->set(r, c, fr);
		}
	}
}

void IMatrix::reduceToSum(IMatrix* op1, IMatrix* op2, std::function<float(float, float)> func, IMatrix* result)
{
	int maxc = std::max(op1->m_Columns, op2->m_Columns);
	for (int r = 0; r < result->m_Rows; ++r) {
		float sum = 0;
		for (int c = 0; c < maxc; ++c) {
			float x1 = op1->get(r, c);
			float x2 = op2->get(r, c);
			float fr = func(x1, x2);
			sum += fr;
		}
		result->set(r, 0, sum);
	}
}

IMatrix* IMatrix::neg(IMatrix* op1)
{
	IMatrix* result = maxMatrix(op1, nullptr);
	auto negFunc = [](float x1) {return -x1; };
	IMatrix::unaryOp(op1, negFunc, result);
	return result;
}

IMatrix* IMatrix::add(IMatrix* op1, IMatrix* op2)
{
	auto addFunc = [](float x1, float x2) {return x1 + x2; };
	IMatrix* result = maxMatrix(op1, op2);
	IMatrix::binaryOp(op1, op2, addFunc, result);
	return result;
}

IMatrix* IMatrix::subtract(IMatrix* op1, IMatrix* op2)
{
	auto addFunc = [](float x1, float x2) {return x1 - x2; };
	IMatrix* result = maxMatrix(op1, op2);
	IMatrix::binaryOp(op1, op2, addFunc, result);
	return result;
}

IMatrix* IMatrix::mul(IMatrix* op1, IMatrix* op2)
{
	auto mulFunc = [](float x1, float x2) {return x1 * x2; };
	IMatrix* result = maxMatrix(op1, op2);
	IMatrix::binaryOp(op1, op2, mulFunc, result);
	return result;
}

IMatrix* IMatrix::quatMul(IMatrix* op1, IMatrix* op2)
{
	float x1 = op1->get(0, 0);
	float y1 = op1->get(0, 1);
	float z1 = op1->get(0, 2);
	float w1 = op1->get(0, 3);

	float x2 = op2->get(0, 0);
	float y2 = op2->get(0, 1);
	float z2 = op2->get(0, 2);
	float w2 = op2->get(0, 3);
	
	float qX = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	float qY = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
	float qZ = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
	float qW = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	Quaternion* result = new Quaternion(qX,qY,qZ,qW);
	return result;
}

IMatrix* IMatrix::divide(IMatrix* op1, IMatrix* op2)
{
	auto addFunc = [](float x1, float x2) {return x1 / x2; };
	IMatrix* result = maxMatrix(op1, op2);
	IMatrix::binaryOp(op1, op2, addFunc, result);
	return result;
}

IMatrix* IMatrix::dot(IMatrix* op1, IMatrix* op2)
{
	if ((op1->m_Type == Type::VECTOR || op1->m_Type == Type::MATRIX)
		&& (op2->m_Type == Type::VECTOR || op2->m_Type == Type::MATRIX))
	{
		IMatrix* result = maxRowMatrix(op1, op2);
		reduceToSum(op1, op2, [](float x1, float x2) {return x1 * x2; }, result);
		return result;
	}
	else {
		return quatMul(op1, op2);
	}
	
}

IMatrix* IMatrix::power(IMatrix* op1, IMatrix* op2)
{
	float power = op2->get(0, 0);
	auto powerFunc = [power](float x1) {return powf(x1,power); };
	IMatrix* result = maxMatrix(op1, nullptr);
	IMatrix::unaryOp(op1, powerFunc, result);
	return result;
}

IMatrix* IMatrix::cross(IMatrix* op1, IMatrix* op2)
{
	// result is always 3D vector.

	IMatrix* result = maxMatrix(op1, op2);
	if (result->m_Columns == 2) {

	}
	return result;
}

bool IMatrix::inRange(int ri, int ci)
{
	return (ri >= 0 && ri < m_Rows&& ci >= 0 && ci < m_Columns);
}
