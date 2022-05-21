#pragma once

TEST_CASE("Constant variable tests", "[constants]") {
	SECTION("Pi") {
		VecMathListener vm;
		vm.exec("Pi=3");
		vm.exec("pitest = Pi");
		checkScalar(vm, "pitest", 3.14159265f);

		vm.exec("PI=4");
		vm.exec("pitest = PI");
		checkScalar(vm, "pitest", 3.14159265f);

		vm.exec("pi=2");
		vm.exec("pitest = pi");
		checkScalar(vm, "pitest", 3.14159265f);
	}

	SECTION("e") {
		VecMathListener vm;
		vm.exec("e=3");
		vm.exec("etest = e");
		checkScalar(vm, "etest", 2.71828182845904f);
	}
}