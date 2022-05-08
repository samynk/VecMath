package dae.gd.vecmathproject;

import dae.gd.vecmathproject.IMatrix.AddressMode;
import dae.gd.vecmathproject.IMatrix.Type;

/**
 * A class to support Quaternion operations.
 *
 * @author Koen.Samyn
 */
public class Quaternion extends IMatrix {

    private float x, y, z, w;

    /**
     * Creates a unit quaternion.
     */
    public Quaternion() {
        this(0, 0, 0, 1);
    }

    /**
     * Creates a quaternion with the provided parameters, defined as : w+ i*x
     * +j*y + k*z
     *
     * @param x the x value of the quaternion.
     * @param y
     * @param z
     * @param w
     */
    public Quaternion(float x, float y, float z, float w) {
        super(Type.QUATERNION, AddressMode.BORDER, 0, 1, 4);
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    /**
     * Returns a sub value of the quaternion. Out of bounds cells will be
     * determined by the address mode property.
     *
     * @param ri the row index.
     * @param ci the column index.
     * @return the value of the cell.
     */
    public float get(int ri, int ci) {
        if (inRange(ri, ci)) {
            switch (ci) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                case 3:
                    return w;
            }
        } else {
            switch (addressMode) {
                case BORDER:
                    return borderValue;
                case CLAMP: {
                    if (ci <= 1) {
                        return x;
                    } else if (ci == 1) {
                        return y;
                    } else if (ci == 1) {
                        return z;
                    } else {
                        return w;
                    }
                }
            }
        }
        return 0.0f;
    }

    /**
     * Sets a value in this quaternion. Out of bounds values will be ignored.
     *
     * @param ri the row index.
     * @param ci the column index.
     * @param value the value to set.
     */
    public void set(int ri, int ci, float value) {
        if (inRange(ri, ci)) {
            switch (ci) {
                case 0:
                    x = value;
                    break;
                case 1:
                    y = value;
                    break;
                case 2:
                    z = value;
                    break;
                case 3:
                    w = value;
                    break;
            }
        }
    }

    /**
     * Prints the value of the quaternion to the console.
     */
    public void print() {
        //SetConsoleTextAttribute(console, 8);
        System.out.print("[");
        //SetConsoleTextAttribute(console, 7);
        System.out.print(w);
        //SetConsoleTextAttribute(console, 8);
        System.out.print(" ,( ");
        //SetConsoleTextAttribute(console, 12);
        System.out.print(x);
        //SetConsoleTextAttribute(console, 8);
        System.out.print(" , ");
        //SetConsoleTextAttribute(console, 10);
        System.out.print(y);
        //SetConsoleTextAttribute(console, 8);
        System.out.print(" , ");
        //SetConsoleTextAttribute(console, 3);
        System.out.print(z);
        //SetConsoleTextAttribute(console, 8);
        System.out.print(" )]");

    }

    /**
     * Calculates the conjugate of this quaternion.
     *
     * @return a new conjugated quaternion.
     */
    public Quaternion conjugate() {
        return new Quaternion(-x, -y, -z, w);
    }

    public Quaternion inverse() {
        float magSqr = x * x + y * y + z * z + w * w;
        return new Quaternion(-x / magSqr, -y / magSqr, -z / magSqr, w / magSqr);
    }
}
