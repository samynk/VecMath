#pragma once
#include "IMatrix.h"
class Scalar : public IMatrix
{
public:
	Scalar();
	Scalar(float value);

	float get(int ri, int ci) override;
	void set(int ri, int ci, float value) override;
private:
	float m_Value{};
};
