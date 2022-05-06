package dae.gd.vecmathproject;

/**
 *
 * @author Koen.Samyn
 */
public class Vector2D extends IMatrix {

    private float x, y;

    public Vector2D() {
        this(0, 0);
    }

    public Vector2D(float x, float y) {
        super(Type.VECTOR, AddressMode.BORDER, 0, 1, 2);
        this.x = x;
        this.y = y;
    }

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
