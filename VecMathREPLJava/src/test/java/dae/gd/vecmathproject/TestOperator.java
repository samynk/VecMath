package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import static dae.gd.vecmathproject.TestUtil.*;

/**
 *
 * @author Koen.Samyn
 */
public class TestOperator {

    public TestOperator() {
    }

    @Test
    public void testPower() {
        VecMathListener vm = new VecMathListener();
        vm.exec("a=5^2");
        checkScalar(vm, "a", 25);

        vm.exec("a2 = 3*5^2");
        checkScalar(vm, "a2", 75);

        vm.exec("a3 = [1,2,3]^3");
        check3DVector(vm, "a3", 1, 8, 27);

        vm.exec("a4 = [2.1,-3]^4");
        check2DVector(vm, "a4", 19.4481f, 81);

        vm.exec("a5=-6^2");
        checkScalar(vm, "a5", -36);
    }
}
