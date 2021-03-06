package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import static dae.gd.vecmathproject.TestUtil.*;
/**
 *
 * @author Koen.Samyn
 */
public class TestVector {

    float x1 = 3;
    float y1 = 7;
    float z1 = 9;
    float x2 = -1;
    float y2 = 3.2f;
    float z2 = 3.3f;

    private VecMathListener listener = new VecMathListener();

    public TestVector() {
    }

    private void initVars() {
        TestUtil.test3DVector(listener, "v1", x1, y1, z1);
        TestUtil.test3DVector(listener, "v2", x2, y2, z2);
    }

    @Test
    public void testVectorFuncs() {
        initVars();
        listener.exec("mv1=|v1|");
        TestUtil.checkScalar(listener, "mv1", (float) Math.sqrt(x1 * x1 + y1 * y1 + z1 * z1));

        listener.exec("mv1=norm(v1)");
        TestUtil.checkScalar(listener, "mv1", (float) Math.sqrt(x1 * x1 + y1 * y1 + z1 * z1));

        listener.exec("mv2=|v2|");
        TestUtil.checkScalar(listener, "mv2", (float) Math.sqrt(x2 * x2 + y2 * y2 + z2 * z2));

        listener.exec("mv2=norm(v2)");
        TestUtil.checkScalar(listener, "mv2", (float) Math.sqrt(x2 * x2 + y2 * y2 + z2 * z2));

        listener.exec("d=v1.v2");
        TestUtil.checkScalar(listener, "d", x1 * x2 + y1 * y2 + z1 * z2);

        listener.exec("a=rad2deg(acos(v1.v2/(|v1|*|v2|)))");
        TestUtil.checkScalar(listener, "a", 27.71332f);
    }

    @Test
    public void testVectorCross() {
        test3DVector(listener, "v1", x1, y1, z1);
        test3DVector(listener, "v2", x2, y2, z2);

        listener.exec("n= v1#v2");
        check3DVector(listener, "n", -5.7f, -18.9f, 16.6f);

        listener.exec("d1=v1.n");
        checkScalar(listener, "d1", 0);
        listener.exec("d2=v2.n");
        checkScalar(listener, "d2", 0);

        listener.exec("nn=n/|n|");
        check3DVector(listener, "nn", -0.22099f, -0.73277f, 0.64359f);

        listener.exec("ln = |nn|");
        checkScalar(listener, "ln", 1.0f);

        test3DVector(listener, "v3", -3.43f, 7.71f, -0.002f);
        listener.exec("n2 = v1#v3");
        check3DVector(listener, "n2", -69.404f, -30.864f, 47.14f);

    }
}
