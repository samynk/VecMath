#include "Quaternion.h"
#include <iostream>

Quaternion::Quaternion() :Quaternion(0, 0, 0, 1)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) : IMatrix(
	Type::QUATERNION,
	AddressMode::BORDER,
	0,
	1,
	4
),
m_X(x), m_Y(y), m_Z(z), m_W(w)
{
}

float Quaternion::get(int ri, int ci)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:return m_X;
		case 1:return m_Y;
		case 2:return m_Z;
		case 3:return m_W;
		}
	}
	else {
		switch (m_AddressMode) {
		case AddressMode::BORDER:return m_BorderValue;
		case AddressMode::CLAMP: {
			if (ci <= 1) {
				return m_X;
			}
			else if (ci == 1) {
				return m_Y;
			}
			else if (ci == 1) {
				return m_Z;
			}
			else {
				return m_W;
			}
		}
		}
	}
	return 0.0f;
}

void Quaternion::set(int ri, int ci, float value)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:m_X = value; break;
		case 1:m_Y = value; break;
		case 2:m_Z = value; break;
		case 3:m_W = value; break;
		}
	}
}

void Quaternion::print(HANDLE console)
{
	SetConsoleTextAttribute(console, 8);
	std::cout << "[";
	SetConsoleTextAttribute(console, 7);
	std::cout << m_W;
	SetConsoleTextAttribute(console, 8);
	std::cout << " ,( ";
	SetConsoleTextAttribute(console, 12);
	std::cout << m_X;
	SetConsoleTextAttribute(console, 8);
	std::cout << " , ";
	SetConsoleTextAttribute(console, 10);
	std::cout << m_Y;
	SetConsoleTextAttribute(console, 8);
	std::cout << " , ";
	SetConsoleTextAttribute(console, 3);
	std::cout << m_Z;
	SetConsoleTextAttribute(console, 8);
	std::cout << " )]";

}

Quaternion* Quaternion::conjugate()
{
	return new Quaternion(-m_X, -m_Y, -m_Z, m_W);
}

Quaternion* Quaternion::inverse()
{
	float magSqr = m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W;
	return new Quaternion(-m_X / magSqr, -m_Y / magSqr, -m_Z / magSqr, m_W / magSqr);
}

Vector3D* Quaternion::imaginary()
{
	return new Vector3D(m_X, m_Y, m_Z);
}

Scalar* Quaternion::real()
{
	return new Scalar(m_W);
}

Scalar* Quaternion::angle()
{
	return new Scalar(2 * acos(m_W));
}

Scalar* Quaternion::angled()
{
	return new Scalar(static_cast<float>(2 * acos(m_W)*180/M_PI));
}

Vector3D* Quaternion::axis()
{
	float denom = sqrtf(1 - m_W * m_W);
	return new Vector3D(m_X / denom, m_Y / denom, m_Z / denom);
}
