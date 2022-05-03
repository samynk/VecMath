#include "Vector2D.h"

Vector2D::Vector2D() :Vector2D(0, 0)
{
}

Vector2D::Vector2D(float x, float y) : IMatrix(
	Type::VECTOR,
	AddressMode::BORDER,
	0,
	1,
	2
),m_X(x),m_Y(y)
{
}

float Vector2D::get(int ri, int ci)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:return m_X;
		case 1:return m_Y;
		}
	}
	else {
		switch (m_AddressMode) {
		case AddressMode::BORDER:return m_BorderValue;
		case AddressMode::CLAMP: {
			if (ci <= 0) {
				return m_X;
			}
			else {
				return m_Y;
			}
		}
		}
	}
	return 0.0f;
}

void Vector2D::set(int ri, int ci, float value)
{
	if (inRange(ri, ci)) {
		switch (ci) {
		case 0:m_X = value; break;
		case 1:m_Y = value; break;
		}
	}
}
