package dae.gd.vecmathproject;

/**
 * Interface for lambdas that defines a unary operation on a matrix.
 * @author Koen.Samyn
 */
public interface UnaryOp {
    /**
     * Calculates the result of a unary function, such as cosine, sine, tangent, ...
     * @param x the input value.
     * @return the calculated value.
     */
    public float calc(float x);
}
