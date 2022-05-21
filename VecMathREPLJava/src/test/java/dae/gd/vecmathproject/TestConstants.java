/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/UnitTests/JUnit5TestClass.java to edit this template
 */
package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import static dae.gd.vecmathproject.TestUtil.*;

/**
 *
 * @author Koen.Samyn
 */
public class TestConstants {

    public TestConstants() {
    }

    @Test
    public void testPI() {
        VecMathListener vm = new VecMathListener();
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

    @Test
    public void testE() {
        VecMathListener vm = new VecMathListener();
        vm.exec("e=3");
        vm.exec("etest = e");
        checkScalar(vm, "etest", 2.71828182845904f);
    }
}
