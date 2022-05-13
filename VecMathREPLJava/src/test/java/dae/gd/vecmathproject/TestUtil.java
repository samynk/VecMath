package dae.gd.vecmathproject;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 *
 * @author Koen.Samyn
 */
public class TestUtil {

    public static void checkScalar(VecMathListener vecMath, String id, float expected) {
        IMatrix var = vecMath.getVariable(id);
        assertNotNull(var);
        float varValue = var.get(0, 0);
        assertEquals(varValue, expected, 1e-4);
    }

    public static void testScalar(VecMathListener vecMath, String id, float expected) {
        vecMath.exec(id + "=" + Float.toString(expected));
        checkScalar(vecMath, id, expected);
    }

    public static void test1DVector(VecMathListener vecMath, String id, float expected) {
        vecMath.exec(id + "=[" + Float.toString(expected) + "]");
        IMatrix var = vecMath.getVariable(id);
        assertNotNull(var);
        float varValue = var.get(0, 0);
        assertEquals(varValue, expected, 1e-4);
    }

    public static void check2DVector(VecMathListener vecMath, String id, float x, float y) {
        IMatrix var = vecMath.getVariable(id);
        assertNotNull(var);
        float xVal = var.get(0, 0);
        float yVal = var.get(0, 1);
        assertEquals(xVal, x, 1e-4);
        assertEquals(yVal, y, 1e-4);
    }

    public static void test2DVector(VecMathListener vecMath, String id, float x, float y) {
        vecMath.exec(id + "=[" + Float.toString(x) + "," + Float.toString(y) + "]");
        check2DVector(vecMath, id, x, y);
    }

    public static void check3DVector(VecMathListener vecMath, String id, float x, float y, float z) {
        IMatrix var = vecMath.getVariable(id);
        assertNotNull(var);
        float xVal = var.get(0, 0);
        float yVal = var.get(0, 1);
        float zVal = var.get(0, 2);
        assertEquals(xVal, x, 1e-4);
        assertEquals(yVal, y, 1e-4);
        assertEquals(zVal, z, 1e-4);
    }

    public static void test3DVector(VecMathListener vecMath, String id, float x, float y, float z) {
        vecMath.exec(id + "=[" + Float.toString(x) + "," + Float.toString(y) + "," + Float.toString(z) + "]");
        check3DVector(vecMath, id, x, y, z);
    }

    public static void checkQuaternion(VecMathListener vecMath, String id, float x, float y, float z, float w) {
        IMatrix var = vecMath.getVariable(id);
        assertNotNull(var);
        float xVal = var.get(0, 0);
        float yVal = var.get(0, 1);
        float zVal = var.get(0, 2);
        float wVal = var.get(0, 3);
        assertEquals(xVal, x, 1e-4);
        assertEquals(yVal, y, 1e-4);
        assertEquals(zVal, z, 1e-4);
        assertEquals(wVal, w, 1e-4);
    }

    public static void testQuaternion(VecMathListener vecMath, String id, float x, float y, float z, float w) {
        String sx = Float.toString(x);
        String sy = Float.toString(y);
        String sz = Float.toString(z);
        String sw = Float.toString(w);
        vecMath.exec(id + "=[" + sw + ",(" + sx + "," + sy + "," + sz + ")]");
        checkQuaternion(vecMath, id, x, y, z, w);
    }
}
