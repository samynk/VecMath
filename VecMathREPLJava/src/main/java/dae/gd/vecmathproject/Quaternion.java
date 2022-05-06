package dae.gd.vecmathproject;

import dae.gd.vecmathproject.IMatrix.AddressMode;
import dae.gd.vecmathproject.IMatrix.Type;

/**
 *
 * @author Koen.Samyn
 */
public class Quaternion extends IMatrix {

    private float x, y, z, w;

    public Quaternion() {
        this(0, 0, 0, 1);
    }

    public Quaternion(float x, float y, float z, float w) {
        super(Type.QUATERNION, AddressMode.BORDER, 0, 1, 4);
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

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

    public void set(int ri, int ci, float value) {
        if (inRange(ri, ci)) {
            switch (ci) {
                case 0:
                    x= value;
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

    public Quaternion conjugate() {
        return new Quaternion(-x, -y, -z, w);
    }
}
