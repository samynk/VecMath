#pragma once
#include "VecMathListener.h"
#include "IMatrix.h"

void testScalar(VecMathListener& vecMath, std::string id, float expected)
{
	vecMath.exec(id+"=" + std::to_string(expected));
	IMatrix* var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float varValue = var->get(0, 0);
	REQUIRE(varValue == Catch::Approx(expected).epsilon(1e-4));
}

void test1DVector(VecMathListener& vecMath, std::string id, float expected)
{
	vecMath.exec(id+"=[" + std::to_string(expected)+"]");
	IMatrix* var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float varValue = var->get(0, 0);
	REQUIRE(varValue == Catch::Approx(expected).epsilon(1e-4));
}

void test2DVector(VecMathListener& vecMath, std::string id, float x,float y)
{
	vecMath.exec(id+"=[" + std::to_string(x) + "," +std::to_string(y) + "]");
	IMatrix* var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float xVal = var->get(0, 0);
	float yVal = var->get(0, 1);
	REQUIRE(xVal == Catch::Approx(x).epsilon(1e-4));
	REQUIRE(yVal == Catch::Approx(y).epsilon(1e-4));
}

TEST_CASE("Assignment tests", "[assign_variables]") {
	SECTION("Simple assignments") {
		VecMathListener listener;
		testScalar(listener, "a", -5.234);
		testScalar(listener, "b", 27.2);
		test1DVector(listener, "a", -24.2137);
		test2DVector(listener, "v1", -9.21, 3.41);
		test2DVector(listener, "v2", 10.87557, -3.41);
	}
}