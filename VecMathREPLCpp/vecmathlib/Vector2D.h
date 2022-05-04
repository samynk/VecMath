#pragma once
#include "IMatrix.h"

class Vector2D:public IMatrix
{
public:
	Vector2D();
	Vector2D(float x, float y);

	float get(int ri, int ci) override;
	void set(int ri, int ci, float value) override;

	IMatrix* conjugate() override;
private:
	float m_X{ 0 };
	float m_Y{ 0 };
};
