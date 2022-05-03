#include "Scalar.h"

Scalar::Scalar() :Scalar(0)
{
}

Scalar::Scalar(float value) :IMatrix(
	Type::VECTOR,
	AddressMode::CLAMP,
	1,
	1
), m_Value(value)
{
}

float Scalar::get(int ri, int ci)
{
	if (inRange(ri, ci)) {
	return m_Value;
	}
	else {
		switch (m_AddressMode) {
		case AddressMode::CLAMP:return m_Value;
		case AddressMode::BORDER:return m_BorderValue;
		case AddressMode::BOUNDSERROR:return NAN;
		}
	}
}

void Scalar::set(int ri, int ci,float value)
{
	if (inRange(ri, ci)) {
		m_Value = value;
	}
}
