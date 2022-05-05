#pragma once
#include "IMatrix.h"
class Quaternion:public IMatrix
{
public:
	Quaternion();
	Quaternion(float x, float y, float z,float w);

	float get(int ri, int ci) override;
	void set(int ri, int ci, float value) override;
	void print(HANDLE console)override;
	Quaternion* conjugate()override;
	Quaternion* inverse()override;
private:
	float m_X{ 0 };
	float m_Y{ 0 };
	float m_Z{ 0 };
	float m_W{ 0 };
};
