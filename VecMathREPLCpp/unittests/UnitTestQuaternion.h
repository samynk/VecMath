#pragma once
#include "VecMathListener.h"
VecMathListener listener;
TEST_CASE("Quaternion tests", "[quaternion_operations]") {
	
	float x1 = 0;
	float y1 = 0;
	float z1 = sin(M_PI / 4);
	float w1 = cos(M_PI) / 4;

	float x3 = 3;
	float y3 = -2;
	float z3 = 8;
	float w3 = 1;

	float x4 = 3;
	float y4 = -2;
	float z4 = 8;
	float w4 = 1;

	SECTION("Create quaternions") {
		testQuaternion(listener, "q1", x1,y1,z1,w1);
		testQuaternion(listener, "q2", -x1,-y1,-z1,w1);
		testQuaternion(listener, "q3", x3, y3, z3, w3);
		testQuaternion(listener, "q4", x4, y4, z4, w4);
	}

	SECTION("Add quaternions") {
		listener.exec("qa1=q1+q2");
		checkQuaternion(listener, "qa1", 0, 0, 0, 2 * w1);
		listener.exec("qa2=q1+q3");
		checkQuaternion(listener, "qa2",x1+x3, y1+y3, z1+z3, w1+w3);
		listener.exec("qa3=q3+q4");
		checkQuaternion(listener, "qa2", x4 + x3, y4 + y3, z4 + z3, w4 + w3);
	}
}
