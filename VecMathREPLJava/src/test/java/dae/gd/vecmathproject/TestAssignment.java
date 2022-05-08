/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/UnitTests/JUnit5TestClass.java to edit this template
 */
package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 *
 * @author Koen.Samyn
 */
public class TestAssignment {

    public TestAssignment() {
    }

    @Test
    public void testAssignment() {
        VecMathListener listener = new VecMathListener();
        TestUtil.testScalar(listener, "a", -5.234f);
        TestUtil.testScalar(listener, "b", 27.2f);
        TestUtil.test1DVector(listener, "a", -24.2137f);
        TestUtil.test2DVector(listener, "v1", -9.21f, 3.41f);
        TestUtil.test2DVector(listener, "v2", 10.87557f, -3.41f);
        TestUtil.test3DVector(listener, "v3", 1.234f, 0.789f, -2.341f);
    }
}
