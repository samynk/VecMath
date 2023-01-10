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

float Quaternion::get(int ri, int ci) const
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

void Quaternion::print(const Console& console) const
{
	console.Print(Console::VMF_GRAY, "[");
	console.Print(Console::VMF_LIGHTGRAY, m_W);
	console.Print(Console::VMF_GRAY, " ,(");
	console.Print(Console::VMF_BRIGHTRED, m_X);
	console.Print(Console::VMF_GRAY, " , ");
	console.Print(Console::VMF_BRIGHTGREEN, m_Y);
	console.Print(Console::VMF_GRAY, " , ");
	console.Print(Console::VMF_CYAN, m_Z);
	console.Print(Console::VMF_GRAY, " )]");
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
	const float m = magnitude();
	return new Scalar(2 * acos(m_W / m));
}

Scalar* Quaternion::angled()
{
	const float m = magnitude();
	return new Scalar(static_cast<float>(2 * acos(m_W / m) * 180 / M_PI));
}

Vector3D* Quaternion::axis()
{
	const float m = magnitude();
	const float n_w = m_W / m;
	const float nom = 1/(m * sqrtf(1 - n_w * n_w));
	return new Vector3D(m_X * nom, m_Y * nom, m_Z * nom);
}
