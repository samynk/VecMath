package dae.gd.vecmathproject;

/**
 * Models a 3D vector object.
 * @author Koen.Samyn
 */
public class Vector3D extends IMatrix {

    private float x, y, z;

    /**
     * Creates a 3d vector object, with the default border address mode.
     */
    public Vector3D() {
        this(0, 0, 0);
    }

    /**
     * Creates a 3d vector object, with the provided x, y and z values.
     * The address mode is BORDER.
     */
    public Vector3D(float x, float y, float z) {
        super(Type.VECTOR, AddressMode.BORDER, 0, 1, 3);
        this.x = x;
        this.y = y;
        this.z = z;
    }

    /**
     * Returns a sub value of the scalar. Out of bounds cells will be determined
     * by the address mode property.
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

    /**
     * Sets the value of this scalar. By default, out of bounds indices are ignored.
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
            }
        }
    }
}
