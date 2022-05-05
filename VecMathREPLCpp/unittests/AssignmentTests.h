#pragma once
#include "VecMathListener.h"
#include "TestUtil.h"
#include "IMatrix.h"



TEST_CASE("Assignment tests", "[assign_variables]") {
	SECTION("Simple assignments") {
		VecMathListener listener;
		testScalar(listener, "a", -5.234);
		testScalar(listener, "b", 27.2);
		test1DVector(listener, "a", -24.2137);
		test2DVector(listener, "v1", -9.21, 3.41);
		test2DVector(listener, "v2", 10.87557, -3.41);
		test3DVector(listener, "v3", 1.234, 0.789, -2.341);
	}
}