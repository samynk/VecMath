package dae.gd.vecmathproject;

/**
 * Models a singular value. This strategy in combination with the address mode
 * makes it easier to calculate the result of matrix operations.
 * @author Koen.Samyn
 */
public class Scalar extends IMatrix {

    private float value;

    /**
     * Creates a scalar with a zero value.
     */
    public Scalar() {
        this(0);
    }

    /**
     * Creates a scaler with the given value and a default CLAMP address mode.
     * @param x the initial value for the scalar.
     */
    public Scalar(float x) {
        super(Type.VECTOR, AddressMode.CLAMP, 1, 1);
        this.value = x;
    }

    /**
     * Returns a sub value of the scalar. Out of bounds cells will be determined
     * by the address mode property.
     * @param ri the row index.
     * @param ci the column index.
     * @return the value of the cell.
     */
    @Override
    public float get(int ri, int ci) {
        if (inRange(ri, ci)) {
            return value;
        } else {
            switch (addressMode) {
                case CLAMP:
                    return value;
                case BORDER:
                    return borderValue;
                case BOUNDSERROR:
                    return Float.NaN;
                default:
                    return Float.NaN;
            }
        }
    }

    /**
     * Sets the value of this scalar. By default, out of bounds indices are ignored.
     * @param ri the row index.
     * @param ci the column index.
     * @param value the value to set.
     */
    @Override
    public void set(int ri, int ci, float value) {
        if (ri == 0 && ci == 0) {
            this.value = value;
        }
    }
}
