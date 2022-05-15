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

float Complex::get(int ri, int ci)
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

void Complex::print(HANDLE console)
{
	SetConsoleTextAttribute(console, 8);
	std::cout << "[";
	SetConsoleTextAttribute(console, 12);
	std::cout << m_R;
	SetConsoleTextAttribute(console, 8);
	std::cout << " ,( ";
	SetConsoleTextAttribute(console, 10);
	std::cout << m_I;
	SetConsoleTextAttribute(console, 8);
	std::cout << " )]";
}
