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
public class TestComplex {

    VecMathListener vm;

    @Test
    public void complexMultiplication() {
        vm.exec("z1=[1,(3)]");
        check2DVector(vm, "z1", 1, 3);
        vm.exec("z2=[3,(4)]");
        check2DVector(vm, "z2", 3, 4);
        vm.exec("z3=z1.z2");
        check2DVector(vm, "z3", -9, 13);
    }

    @Test
    public void inverse() {
        vm.exec("z1=[-3,(8.2)]");
        vm.exec("z2=inv(z1)");
        check2DVector(vm, "z2", -0.03935f, -0.10756f);
        vm.exec("z3=z1.z2");
        check2DVector(vm, "z3", 1, 0);

        vm.exec("z1norm=|z1|");
        checkScalar(vm, "z1norm", 8.73155f);
    }

    @Test
    public void Conjugate() {
        vm.exec("z1=[-3,(8.2)]");
        vm.exec("z2=con(z1)");
        check2DVector(vm, "z2", -3, -8.2f);
    }

    @Test
    public void imaginaryAndReal() {
        vm.exec("q1=[1,(2)]");
        vm.exec("r=re(q1)");
        checkScalar(vm, "r", 1);
        vm.exec("i=im(q1)");
        checkScalar(vm, "i", 2);
    }
}
