#pragma once
#include "IMatrix.h"
class Scalar : public IMatrix
{
public:
	Scalar();
	Scalar(float value);

	float get(int ri, int ci) const override;
	void set(int ri, int ci, float value) override;

	IMatrix* conjugate() override;
private:
	float m_Value{};
};
