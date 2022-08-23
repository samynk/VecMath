#pragma once
#include "VecMathListener.h"
#include "TestUtil.h"
#include "IMatrix.h"



TEST_CASE("Memory tests", "[assignment memory]") {
	SECTION("Simple assignments") {
		size_t matrixObjects = IMatrix::m_Count;
		VecMathListener listener;
		testScalar(listener, "a", -5.234f);
		checkMemory(listener, matrixObjects);
	}
}
