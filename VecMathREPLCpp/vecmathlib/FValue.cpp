#include "FValue.h"
#include <iostream>

FValue::FValue():FValue(0)
{
}

FValue::FValue(float x)
	:m_Values(new float[1]), m_Dimension(1)
{
	m_Values[0] = x;
}

FValue::FValue(float x, float y)
	:m_Values(new float[2]), m_Dimension(2)
{
	m_Values[0] = x;
	m_Values[1] = y;
}

FValue::FValue(float x, float y, float z)
	:m_Values(new float[3]),m_Dimension(3)
{
	m_Values[0] = x;
	m_Values[1] = y;
	m_Values[2] = z;
}

FValue::FValue(const FValue& other)
{
	m_Dimension = other.m_Dimension;
	m_Values = new float[m_Dimension];
	for (int i = 0; i < m_Dimension; ++i) {
		m_Values[i] = other.m_Values[i];
	}
	
}

FValue& FValue::operator=(FValue other)
{
	m_Dimension = other.m_Dimension;
	m_Values = new float[m_Dimension];
	for (int i = 0; i < m_Dimension; ++i) {
		m_Values[i] = other.m_Values[i];
	}
	return *this;
}

FValue::FValue(FValue&& other) noexcept
{
	m_Dimension = other.m_Dimension;
	m_Values = other.m_Values;
	other.m_Values = nullptr;

}

FValue& FValue::operator=(FValue&& rhs) noexcept
{
	m_Dimension = rhs.m_Dimension;
	if (m_Values != nullptr) {
		delete[] m_Values;
	}
	m_Values = rhs.m_Values;
	rhs.m_Values = nullptr;
	return *this;
}

void FValue::print() {
	std::cout << "[";
	for (int i = 0; i < m_Dimension; ++i) {
		std::cout << m_Values[i];
		if (i + 1 < m_Dimension) {
			std::cout << ",";
		}
	}
	std::cout << "]";
}
