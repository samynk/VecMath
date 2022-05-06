package dae.gd.vecmathproject;

/**
 *
 * @author Koen.Samyn
 */
public class Vector3D extends IMatrix {

    private float x, y, z;

    public Vector3D() {
        this(0, 0, 0);
    }

    public Vector3D(float x, float y, float z) {
        super(Type.VECTOR, AddressMode.BORDER, 0, 1, 3);
        this.x = x;
        this.y = y;
        this.z = z;
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
                    } else {
                        return z;
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
                    x = value;
                    break;
                case 1:
                    y = value;
                    break;
                case 2:
                    z = value;
                    break;
            }
        }
    }
}
