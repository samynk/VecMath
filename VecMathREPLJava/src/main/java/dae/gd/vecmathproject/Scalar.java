package dae.gd.vecmathproject;

/**
 *
 * @author Koen.Samyn
 */
public class Scalar extends IMatrix {

    private float value;

    public Scalar() {
        this(0);
    }

    public Scalar(float x) {
        super(Type.VECTOR, AddressMode.CLAMP, 1, 1);
        this.value = x;
    }

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

    @Override
    public void set(int ri, int ci, float value) {
        if (ri == 0 && ci == 0) {
            this.value = value;
        }
    }
}
