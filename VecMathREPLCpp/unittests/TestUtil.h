#pragma once


void checkScalar(VecMathListener& vecMath, std::string id, float expected) {
	auto var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float varValue = var->get(0, 0);
	REQUIRE(varValue == Catch::Approx(expected).epsilon(1e-4));
}

void testScalar(VecMathListener& vecMath, std::string id, float expected)
{
	vecMath.exec(id + "=" + std::to_string(expected));
	checkScalar(vecMath, id, expected);
}

void test1DVector(VecMathListener& vecMath, std::string id, float expected)
{
	vecMath.exec(id + "=[" + std::to_string(expected) + "]");
	auto var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float varValue = var->get(0, 0);
	REQUIRE(varValue == Catch::Approx(expected).epsilon(1e-4));
}

void check2DVector(VecMathListener& vecMath, std::string id, float x, float y)
{
	auto var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float xVal = var->get(0, 0);
	float yVal = var->get(0, 1);
	REQUIRE(xVal == Catch::Approx(x).epsilon(1e-4));
	REQUIRE(yVal == Catch::Approx(y).epsilon(1e-4));
}

void test2DVector(VecMathListener& vecMath, std::string id, float x, float y)
{
	vecMath.exec(id + "=[" + std::to_string(x) + "," + std::to_string(y) + "]");
	check2DVector(vecMath, id, x, y);
}

void check3DVector(VecMathListener& vecMath, std::string id, float x, float y, float z)
{
	auto var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float xVal = var->get(0, 0);
	float yVal = var->get(0, 1);
	float zVal = var->get(0, 2);
	REQUIRE(xVal == Catch::Approx(x).epsilon(1e-4));
	REQUIRE(yVal == Catch::Approx(y).epsilon(1e-4));
	REQUIRE(zVal == Catch::Approx(z).epsilon(1e-4));
}

void test3DVector(VecMathListener& vecMath, std::string id, float x, float y, float z)
{
	vecMath.exec(id + "=[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]");
	check3DVector(vecMath, id, x, y, z);
}

void checkQuaternion(VecMathListener& vecMath, std::string id, float x, float y, float z, float w) {
	auto var = vecMath.getVariable(id);
	REQUIRE(var != nullptr);
	float xVal = var->get(0, 0);
	float yVal = var->get(0, 1);
	float zVal = var->get(0, 2);
	float wVal = var->get(0, 3);
	REQUIRE(xVal == Catch::Approx(x).epsilon(1e-4));
	REQUIRE(yVal == Catch::Approx(y).epsilon(1e-4));
	REQUIRE(zVal == Catch::Approx(z).epsilon(1e-4));
	REQUIRE(wVal == Catch::Approx(w).epsilon(1e-4));
}

void testQuaternion(VecMathListener& vecMath, std::string id, float x, float y, float z, float w)
{
	std::string sx = std::to_string(x);
	std::string sy = std::to_string(y);
	std::string sz = std::to_string(z);
	std::string sw = std::to_string(w);
	vecMath.exec(id + "=[" + sw + ",(" +sx + "," + sy + "," + sz + ")]");
	checkQuaternion(vecMath, id, x, y, z, w);
}

void checkMemory(VecMathListener& vecMath,size_t expected)
{
	vecMath.clearVariables();
	REQUIRE(IMatrix::m_Count == expected);
}

