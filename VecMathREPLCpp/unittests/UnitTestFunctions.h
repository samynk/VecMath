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
        checkQuaternion(vm, "absq", 3.14f, 1, 6.2f,1);
	}
}