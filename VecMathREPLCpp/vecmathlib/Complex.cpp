#include "Complex.h"
#include <iostream>

Complex::Complex():Complex(0,0)
{
}

Complex::Complex(float x, float y):IMatrix(
	Type::COMPLEX,
	AddressMode::BORDER,
	0,
	1,
	2
), m_R(x), m_I(y)
{

}

float Complex::get(int ri, int ci) const
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:return m_R;
		case 1:return m_I;
		}
	}
	else {
		switch (m_AddressMode) {
		case AddressMode::BORDER:return m_BorderValue;
		case AddressMode::CLAMP: {
			if (ci <= 0) {
				return m_R;
			}
			else {
				return m_I;
			}
		}
		}
	}
	return 0.0f;
}

void Complex::set(int ri, int ci, float value)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:m_R = value; break;
		case 1:m_I = value; break;
		}
	}
}

Complex* Complex::conjugate()
{
	return new Complex(m_R,-m_I);
}

Complex* Complex::inverse()
{
	float sqrNorm = m_R * m_R + m_I * m_I;
	return new Complex(m_R / sqrNorm, -m_I / sqrNorm);
}

Scalar* Complex::real()
{
	return new Scalar(m_R);
}

Scalar* Complex::imaginary()
{
	return new Scalar(m_I);
}

void Complex::print(const Console& console) const
{
	console.Print(Console::VMF_GRAY, "[");
	console.Print(Console::VMF_BRIGHTRED, m_R);
	console.Print(Console::VMF_GRAY, " ,(");
	console.Print(Console::VMF_BRIGHTGREEN, m_I);
	console.Print(Console::VMF_GRAY, " )]");
}
