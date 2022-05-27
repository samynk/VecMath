#pragma once
TEST_CASE("Function tests", "[mathematical_functions]") {
	SECTION("Absolute value") {
		VecMathListener vm;

		vm.exec("a=-3");
		checkScalar(vm, "a", -3);
		vm.exec("absa=abs(a)");
		checkScalar(vm, "absa", 3);

		vm.exec("b=abs([9,-3.2])");
		check2DVector(vm, "b", 9, 3.2f);

		vm.exec("c=[9,-3.2,-2.107]");
		vm.exec("absc=abs(c)");
		check3DVector(vm, "absc", 9, 3.2f, 2.107f);

		vm.exec("q=[-1,(-3.14,-1,6.2)]");
		vm.exec("absq=abs(q)");
		checkQuaternion(vm, "absq", 3.14f, 1, 6.2f, 1);
	}

	SECTION("Trigonometric Degrees") {
		VecMathListener vm;
		vm.exec("s1=sind(31)");
		checkScalar(vm, "s1", 0.515038f);
		vm.exec("c1=cosd(127)");
		checkScalar(vm, "c1", -0.601815f);
		vm.exec("t1=tand(-77)");
		checkScalar(vm, "t1", -4.33147587f);

		vm.exec("as1=asind(0.43)");
		checkScalar(vm, "as1", 25.4675601f);
		vm.exec("as2=asind(-0.21)");
		checkScalar(vm, "as2", -12.122352f);

		vm.exec("ac1=acosd(0.89)");
		checkScalar(vm, "ac1", 27.1267531f);
		vm.exec("ac2=acosd(-0.67)");
		checkScalar(vm, "ac2", 132.0670648f);

		vm.exec("at1=atand(1.78)");
		checkScalar(vm, "at1", 60.6728205f);
		vm.exec("at2=atand(-3.189)");
		checkScalar(vm, "at2", -72.58972f);
	}

	SECTION("Non existant functions") {
		// test will fail if this results in crash.
		VecMathListener vm;
		vm.exec("q=[1,(2,4,5)]");
		vm.exec("r(q)");
		vm.exec("r1=r(q)");
		IMatrix* var = vm.getVariable("r1");
		REQUIRE(var == nullptr);
	}
}