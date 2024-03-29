#pragma once
#include "IMatrix.h"
#include "Scalar.h"
class Complex: public IMatrix
{
public:
	Complex();
	Complex(float x, float y);

	float get(int ri, int ci) const override;
	void set(int ri, int ci, float value) override;
	void print(const Console& console) const override;
	
	Complex* conjugate()override;
	Complex* inverse()override;
	Scalar* real() override;
	Scalar* imaginary() override;
private:
	float m_R{ 0 };
	float m_I{ 0 };
};
