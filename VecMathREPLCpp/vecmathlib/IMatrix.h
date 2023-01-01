#pragma once
#define _USE_MATH_DEFINES
#define NOMINMAX
#include "math.h"
#include <functional>
#include <windows.h>

enum class Type { MATRIX, VECTOR, COMPLEX, QUATERNION };
enum class AddressMode { CLAMP, BORDER, BOUNDSERROR };

class IMatrix
{
public:
	IMatrix(Type type, AddressMode mode, int rows, int columns);
	IMatrix(Type type, AddressMode mode, float borderValue, int rows, int columns);
	~IMatrix();

	virtual float get(int ri, int ci) = 0;
	virtual void set(int ri, int ci, float value) = 0;

	virtual void print(HANDLE console);
	virtual float magnitude();
	virtual IMatrix* conjugate();
	virtual IMatrix* inverse();
	virtual IMatrix* real();
	virtual IMatrix* imaginary();
	virtual IMatrix* angle();
	virtual IMatrix* angled();
	virtual IMatrix* axis();


	static IMatrix* maxMatrix(IMatrix* op1, IMatrix* op2);
	static IMatrix* maxRowMatrix(IMatrix* op1, IMatrix* op2);
	static IMatrix* maxColumnMatrix(IMatrix* op1, IMatrix* op2);
	static void binaryOp(IMatrix* op1,
		IMatrix* op2,
		std::function<float(float, float)>func,
		IMatrix* result);
	static void unaryOp(IMatrix* op1,
		std::function<float(float)>func,
		IMatrix* result);
	static void reduceToSum(IMatrix* op1,
		IMatrix* op2,
		std::function<float(float, float)>func,
		IMatrix* result);


	static IMatrix* neg(IMatrix* op1);
	static IMatrix* add(IMatrix* op1, IMatrix* op2);
	static IMatrix* subtract(IMatrix* op1, IMatrix* op2);
	static IMatrix* mul(IMatrix* op1, IMatrix* op2);
	static IMatrix* quatMul(IMatrix* op1, IMatrix* op2);
	static IMatrix* complexMul(IMatrix* op1, IMatrix* op2);
	static IMatrix* divide(IMatrix* op1, IMatrix* op2);
	static IMatrix* dot(IMatrix* op1, IMatrix* op2);
	static IMatrix* power(IMatrix* op1, IMatrix* op2);
	static IMatrix* cross(IMatrix* op1, IMatrix* op2);

	static int m_Count;
protected:
	bool inRange(int ri, int ci) const;
	Type m_Type;
	AddressMode m_AddressMode;
	float m_BorderValue{ 0 };

private:
	int m_Rows{ 0 };
	int m_Columns{ 0 };
};
