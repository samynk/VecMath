package dae.gd.vecmathproject;

/**
 *
 * @author Koen.Samyn
 */
public class Complex extends IMatrix {

    private float r, i;

    public Complex() {
        this(0, 0);
    }

    public Complex(float r, float i) {
        super(IMatrix.Type.COMPLEX, IMatrix.AddressMode.BORDER, 0, 1, 2);
        this.r = r;
        this.i = i;

    }

    public float get(int ri, int ci) {
        if (inRange(ri, ci)) {
            switch (ci) {
                case 0:
                    return r;
                case 1:
                    return i;
            }
        } else {
            switch (addressMode) {
                case BORDER:
                    return borderValue;
                case CLAMP: {
                    if (ci <= 0) {
                        return r;
                    } else {
                        return i;
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
                    r = value;
                    break;
                case 1:
                    i = value;
                    break;
            }
        }
    }

    public Complex conjugate() {
        return new Complex(r, -i);
    }

    public Complex inverse() {
        float sqrNorm = r * r + i * i;
        return new Complex(r / sqrNorm, -i / sqrNorm);
    }
    
    @Override
    public Scalar real() {
        return new Scalar(r);
    }

    @Override
    public Scalar imaginary() {
        return new Scalar(i);
    }
}
