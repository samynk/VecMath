#pragma once
#include "IMatrix.h"
#include "Vector3D.h"
#include "Scalar.h"
class Quaternion:public IMatrix
{
public:
	Quaternion();
	Quaternion(float x, float y, float z,float w);

	float get(int ri, int ci)  const override;
	void set(int ri, int ci, float value) override;
	void print(const Console& console) const override;
	Quaternion* conjugate()override;
	Quaternion* inverse()override;
	Vector3D* imaginary() override;
	Scalar* real() override;
	Scalar* angle() override;
	Scalar* angled() override;
	Vector3D* axis() override;
private:
	float m_X{ 0 };
	float m_Y{ 0 };
	float m_Z{ 0 };
	float m_W{ 0 };
};
