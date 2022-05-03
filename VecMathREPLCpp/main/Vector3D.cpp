#include "Vector3D.h"

Vector3D::Vector3D() :Vector3D(0, 0,0)
{
}

Vector3D::Vector3D(float x, float y,float z) : IMatrix(
	Type::VECTOR,
	AddressMode::BORDER,
	0,
	1,
	3
),
m_X(x),m_Y(y),m_Z(z)
{
}

float Vector3D::get(int ri, int ci)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:return m_X;
		case 1:return m_Y;
		case 2:return m_Z;
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
			else {
				return m_Z;
			}
		}
		}
	}
	return 0.0f;
}

void Vector3D::set(int ri, int ci, float value)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:m_X = value; break;
		case 1:m_Y = value; break;
		case 2:m_Z = value; break;
		}
	}
}