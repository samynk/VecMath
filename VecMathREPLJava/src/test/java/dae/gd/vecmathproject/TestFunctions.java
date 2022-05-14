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
        checkScalar(listener,"a",-3);
        listener.exec("absa=abs(a)");
        checkScalar(listener,"absa",3);
        
        listener.exec("b=abs([9,-3.2])");
        check2DVector(listener,"b",9,3.2f);
        
        listener.exec("c=[9,-3.2,-2.107]");
        listener.exec("absc=abs(c)");
        check3DVector(listener,"absc",9,3.2f,2.107f);
        
        listener.exec("q=[-1,(-3.14,-1,6.2)]");
        listener.exec("absq=abs(q)");
        checkQuaternion(listener, "absq", 3.14f,1,6.2f,1);
    }
}
