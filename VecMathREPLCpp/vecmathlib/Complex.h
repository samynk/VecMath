#pragma once
#include "IMatrix.h"
class Complex: public IMatrix
{
public:
	Complex();
	Complex(float x, float y);

	float get(int ri, int ci) override;
	void set(int ri, int ci, float value) override;
	void print(HANDLE console)override;
	
	Complex* conjugate()override;
	Complex* inverse()override;
private:
	float m_R{ 0 };
	float m_I{ 0 };
};
