package dae.gd.vecmathproject;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.BeforeAll;

/**
 *
 * @author Koen.Samyn
 */
public class TestQuaternion {

    public TestQuaternion() {
    }

    float x1 = 0;
    float y1 = 0;
    float z1 = (float) Math.sin(Math.PI / 4);
    float w1 = (float) Math.cos(Math.PI / 4);

    float x3 = 3.8771f;
    float y3 = -2.9248f;
    float z3 = 8.190f;
    float w3 = 1;
    // (3.8771,-2.9248,8.190,1)

    float x4 = -1.3f;
    float y4 = 7.2f;
    float z4 = 23;
    float w4 = 6.10f;
    // (-1.3,7.2,23,6.10)

    static VecMathListener listener = new VecMathListener();

    public void initVars() {

        TestUtil.testQuaternion(listener, "q1", x1, y1, z1, w1);
        TestUtil.testQuaternion(listener, "q2", -x1, -y1, -z1, w1);
        TestUtil.testQuaternion(listener, "q3", x3, y3, z3, w3);
        TestUtil.testQuaternion(listener, "q4", x4, y4, z4, w4);
    }

    @Test
    public void addQuaternions() {
        initVars();
        listener.exec("qa1=q1+q2");
        TestUtil.checkQuaternion(listener, "qa1", 0, 0, 0, 2 * w1);
        listener.exec("qa2=q1+q3");
        TestUtil.checkQuaternion(listener, "qa2", x1 + x3, y1 + y3, z1 + z3, w1 + w3);
        listener.exec("qa3=q3+q4");
        TestUtil.checkQuaternion(listener, "qa3", x4 + x3, y4 + y3, z4 + z3, w4 + w3);
    }

    @Test
    public void multiplyQuaternions() {
        initVars();
        listener.exec("q12=q1.q2");
        TestUtil.checkQuaternion(listener, "q12", 0, 0, 0, 1);
        listener.exec("q34=q3.q4");
        TestUtil.checkQuaternion(listener, "q34", -103.88809f, -110.46158f, +97.07188f, -156.17121f);
        listener.exec("q43=q4.q3");
        TestUtil.checkQuaternion(listener, "q43", 148.58871f, 89.17902f, 48.84612f, -156.17121f);
    }

    @Test
    public void normOfQuaternions() {
        initVars();
        listener.exec("mq1=|q1|");
        TestUtil.checkScalar(listener, "mq1", 1);

        listener.exec("mq1=norm(q1)");
        TestUtil.checkScalar(listener, "mq1", 1);

        listener.exec("mq2=|q2|");
        TestUtil.checkScalar(listener, "mq2", 1);

        listener.exec("mq2=norm(q2)");
        TestUtil.checkScalar(listener, "mq2", 1);

        listener.exec("mq3=|q3|");
        TestUtil.checkScalar(listener, "mq3", 9.574f);

        listener.exec("mq3=norm(q3)");
        TestUtil.checkScalar(listener, "mq3", 9.574f);
    }

    @Test
    public void inverseOfQuaternions() {
        initVars();
        listener.exec("iq3=inv(q3)");
        TestUtil.checkQuaternion(listener, "iq3", -0.04229758f, 0.03190837f, -0.08934956f, 0.01090959f);

        listener.exec("iq4=inv(q4)");
        TestUtil.checkQuaternion(listener, "iq4", 0.0020976f, -0.011618f, -0.037112f, 0.009843f);
    }

    @Test
    public void quaternionRotation() {
        listener.exec("q1=[cos(Pi/4),(0,0,sin(Pi/4))]");
        listener.exec("p=[1,0,0]");
        listener.exec("pr=q1.p.con(q1)");
        TestUtil.checkQuaternion(listener, "pr", 0, 1, 0, 0);
    }
}
