#pragma once
#include "VecMathListener.h"
#include "TestUtil.h"
#include "IMatrix.h"



TEST_CASE("Assignment tests", "[assign_variables]") {
	SECTION("Simple assignments") {
		VecMathListener listener;
		testScalar(listener, "a", -5.234f);
		testScalar(listener, "b", 27.2f);
		test1DVector(listener, "a", -24.2137f);
		test2DVector(listener, "v1", -9.21f, 3.41f);
		test2DVector(listener, "v2", 10.87557f, -3.41f);
		test3DVector(listener, "v3", 1.234f, 0.789f, -2.341f);
		//checkMemory(listener);
	}
}