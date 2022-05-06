package dae.gd.vecmathproject;

/**
 * Interface for lambdas that defines a binary operation on a matrix.
 * @author Koen.Samyn
 */
public interface BinaryOp {
    /**
     * Calculates the result of a binary function, such as add, multiply, ...
     * @param x1 the first operand.
     * @param x2 the second operand.
     * @return the calculated value.
     */
    float calc(float x1, float x2);
}
