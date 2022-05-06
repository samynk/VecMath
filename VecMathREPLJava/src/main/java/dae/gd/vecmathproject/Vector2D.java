package dae.gd.vecmathproject;

/**
 * Models a 2D vector.
 * @author Koen.Samyn
 */
public class Vector2D extends IMatrix {

    private float x, y;

    /**
     * Creates a 2d vector with x and y set to zero. The default address mode
     * is BORDER.
     */
    public Vector2D() {
        this(0, 0);
    }

    /**
     * Creates a 2D vector with the provided x and y. The default address mode
     * is BORDER.
     * @param x the initial value for x.
     * @param y the initial value for y.
     */
    public Vector2D(float x, float y) {
        super(Type.VECTOR, AddressMode.BORDER, 0, 1, 2);
        this.x = x;
        this.y = y;
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
            }
        } else {
            switch (addressMode) {
                case BORDER:
                    return borderValue;
                case CLAMP: {
                    if (ci <= 0) {
                        return x;
                    } else {
                        return y;
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
            }
        }
    }
}
