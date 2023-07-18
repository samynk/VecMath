#pragma once
#include "IMatrix.h"
class Vector3D :public IMatrix
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	float get(int ri, int ci) const override;
	void set(int ri, int ci, float value) override;

	IMatrix* conjugate() override;
private:
	float m_X{ 0 };
	float m_Y{ 0 };
	float m_Z{ 0 };
};
