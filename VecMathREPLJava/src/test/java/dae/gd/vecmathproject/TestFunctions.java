package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import static dae.gd.vecmathproject.TestUtil.*;

/**
 *
 * @author Koen.Samyn
 */
public class TestFunctions {

    VecMathListener listener = new VecMathListener();

    public TestFunctions() {
    }

    @Test
    public void testABS() {
        listener.exec("a=-3");
        checkScalar(listener, "a", -3);
        listener.exec("absa=abs(a)");
        checkScalar(listener, "absa", 3);

        listener.exec("b=abs([9,-3.2])");
        check2DVector(listener, "b", 9, 3.2f);

        listener.exec("c=[9,-3.2,-2.107]");
        listener.exec("absc=abs(c)");
        check3DVector(listener, "absc", 9, 3.2f, 2.107f);

        listener.exec("q=[-1,(-3.14,-1,6.2)]");
        listener.exec("absq=abs(q)");
        checkQuaternion(listener, "absq", 3.14f, 1, 6.2f, 1);
    }

    @Test
    public void testTrigonometricDegrees() {
        VecMathListener vm = new VecMathListener();
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
}
