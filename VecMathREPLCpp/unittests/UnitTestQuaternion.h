#pragma once
#include "VecMathListener.h"
VecMathListener listener;
TEST_CASE("Quaternion tests", "[quaternion_operations]") {

	float x1 = 0;
	float y1 = 0;
	float z1 = sin(static_cast<float>(M_PI / 4));
	float w1 = cos(static_cast<float>(M_PI / 4));

	float x3 = 3.8771f;
	float y3 = -2.9248f;
	float z3 = 8.190f;
	float w3 = 1;
	// (3.8771,-2.9248,8.190,1)

	float x4 = -1.3f;
	float y4 = 7.2f;
	float z4 = 23;
	float w4 = 6.10f;
	// (-1.3,7.2,23,6.10)

	SECTION("Create quaternions") {
		testQuaternion(listener, "q1", x1, y1, z1, w1);
		testQuaternion(listener, "q2", -x1, -y1, -z1, w1);
		testQuaternion(listener, "q3", x3, y3, z3, w3);
		testQuaternion(listener, "q4", x4, y4, z4, w4);
	}

	SECTION("Add quaternions") {
		listener.exec("qa1=q1+q2");
		checkQuaternion(listener, "qa1", 0, 0, 0, 2 * w1);
		listener.exec("qa2=q1+q3");
		checkQuaternion(listener, "qa2", x1 + x3, y1 + y3, z1 + z3, w1 + w3);
		listener.exec("qa3=q3+q4");
		checkQuaternion(listener, "qa3", x4 + x3, y4 + y3, z4 + z3, w4 + w3);
	}

	SECTION("Multiply quaternions") {
		listener.exec("q12=q1.q2");
		checkQuaternion(listener, "q12", 0, 0, 0, 1);
		listener.exec("q34=q3.q4");
		checkQuaternion(listener, "q34", -103.88809f, -110.46158f, +97.07188f, -156.17121f);
		listener.exec("q43=q4.q3");
		checkQuaternion(listener, "q43", 148.58871f, 89.17902f, 48.84612f, -156.17121f);
	}

	SECTION("Norm of quaternions") {
		listener.exec("mq1=|q1|");
		checkScalar(listener, "mq1", 1);

		listener.exec("mq1=norm(q1)");
		checkScalar(listener, "mq1", 1);

		listener.exec("mq2=|q2|");
		checkScalar(listener, "mq2", 1);

		listener.exec("mq2=norm(q2)");
		checkScalar(listener, "mq2", 1);

		listener.exec("mq3=|q3|");
		checkScalar(listener, "mq3", 9.574f);

		listener.exec("mq3=norm(q3)");
		checkScalar(listener, "mq3", 9.574f);
	}

	SECTION("Inverse of quaternions") {
		listener.exec("iq3=inv(q3)");
		checkQuaternion(listener, "iq3", -0.04229758f, 0.03190837f, -0.08934956f, 0.01090959f);

		listener.exec("iq4=inv(q4)");
		checkQuaternion(listener, "iq4", 0.0020976f, -0.011618f, -0.037112f, 0.009843f);
	}

	SECTION("Quaternion rotation") {
		listener.exec("q1=[cos(Pi/4),(0,0,sin(Pi/4))]");
		listener.exec("p=[1,0,0]");
		listener.exec("pr=q1.p.con(q1)");
		checkQuaternion(listener, "pr", 0, 1, 0, 0);
	}

	SECTION("Imaginary and real parts") {
		listener.exec("q1=[1,(2,3,4)]");
		listener.exec("w=re(q1)");
		checkScalar(listener, "w", 1);
		listener.exec("v=im(q1)");
		check3DVector(listener, "v", 2, 3, 4);
	}

	SECTION("Conversion to axis angle") {
		listener.exec("q1=[cosd(33),(0,sind(33),0)]");
		listener.exec("arad=angle(q1)");
		listener.exec("adeg=angled(q1)");
		listener.exec("axis=axis(q1)");

		checkScalar(listener, "arad", static_cast<float>(66 * M_PI / 180));
		checkScalar(listener, "adeg", static_cast<float>(66));
		check3DVector(listener, "axis", 0, 1, 0);

		float x1 = 3;
		float y1 = 7;
		float z1 = -1;
		float norm = sqrt(x1*x1+y1*y1+z1*z1);
		float nx1 = x1 / norm;
		float ny1 = y1 / norm;
		float nz1 = z1 / norm;
		
		float a2rad = static_cast<float>(21 * M_PI / 180);
		float s = sin(a2rad);

		listener.exec("q1=[cosd(21),("
			+ std::to_string(nx1 * s)
			+ ","
			+ std::to_string(ny1 * s)
			+ ","
			+ std::to_string(nz1 * s)
			+ ")]");
		listener.exec("a2rad=angle(q1)");
		listener.exec("a2deg=angled(q1)");
		listener.exec("axis2=axis(q1)");

		checkScalar(listener,"a2rad",2*a2rad);
		checkScalar(listener, "a2deg", 42);
		check3DVector(listener, "axis2", nx1, ny1, nz1);
	}
}
