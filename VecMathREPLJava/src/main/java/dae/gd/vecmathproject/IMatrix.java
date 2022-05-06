package dae.gd.vecmathproject;

/**
 * An abstract class that models a two-dimensional matrix.
 * The class supports the implementation of vectors, quaternions
 * and transformation matrices.
 * The addressmode feature of the matrix makes it possible to define
 * what happens when a cell is accessed that is outside of the bounds
 * of the matrix. The resulting value can be clamped, or a constant border
 * value can be defined, or the operation can result in an error.
 * @author Koen.Samyn
 */
public abstract class IMatrix {

    private int rows;
    private int columns;
    protected Type type = Type.VECTOR;
    protected AddressMode addressMode = AddressMode.CLAMP;
    protected float borderValue = 0;

    /**
     * The type of the matrix
     */
    public enum Type {
        MATRIX, VECTOR, QUATERNION
    };

    /**
     * The address mode of the matrix.
     */
    public enum AddressMode {
        CLAMP, BORDER, BOUNDSERROR
    };

    /**
     * Creates a new matrix of a given type, address mode and a given
     * number of rows of columns.
     * @param type the type of the matrix, can be matrix, vector or quaternion.
     * @param mode the mode of the matrix, either clamp, border or boundserror.
     * @param rows the number of rows in this matrix.
     * @param columns the number of columns in this matrix.
     */
    public IMatrix(Type type, AddressMode mode, int rows, int columns) {
        this(type,mode,0,rows,columns);
    }

    /**
     * Creates a new matrix of a given type, address mode and a given
     * number of rows of columns.
     * @param type the type of the matrix, can be matrix, vector or quaternion.
     * @param mode the mode of the matrix, either clamp, border or boundserror.
     * @param borderValue the constant value that will be returned if an out of bounds
     * cell value is accessed.
     * @param rows the number of rows in this matrix.
     * @param columns the number of columns in this matrix.
     */
    public IMatrix(Type type, AddressMode mode, float borderValue, int rows, int columns) {
        this.type = type;
        this.addressMode = mode;
        this.borderValue = borderValue;
        this.rows = rows;
        this.columns = columns;
    }

    /**
     * Gets a value from this matrix.
     * @param ri the index of the row (zero based).
     * @param ci the index of the column (zero based).
     * @return the resulting value of the cell, taking into account the address mode.
     */
    public abstract float get(int ri, int ci);

    /**
     * Sets a value in this matrix. If the row index and column index is out of
     * bounds no operation will be executed.
     * @param ri the index of the row (zero based).
     * @param ci the index of the column (zero based).
     * @param value the value to set.
     */
    public abstract void set(int ri, int ci, float value);

    /**
     * Prints the contents of this matrix to the console.
     */
    public void print() {
        //SetConsoleTextAttribute(console, 8);
        if (rows > 1) {
            System.out.print("{");
        }
        for (int r = 0; r < rows; ++r) {
            System.out.print("[");
            for (int c = 0; c < columns; ++c) {
                //SetConsoleTextAttribute(console, 10);
                System.out.print(get(r, c));
                if (c + 1 < columns) {
                    //SetConsoleTextAttribute(console, 8);
                    System.out.print(",");
                }
            }
            if (r + 1 < rows) {
                //SetConsoleTextAttribute(console, 8);
                System.out.print(",");
            }
            //SetConsoleTextAttribute(console, 8);
            System.out.print("]");
        }
        if (rows > 1) {
            //SetConsoleTextAttribute(console, 8);
            System.out.print("}");
        }
    }

    /**
     * The square root of the sum of squares of all the elements in this matrix.
     * @return the magnitude of this matrix.
     */
    public float magnitude() {
        float sum = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < columns; ++c) {
                float value = get(r, c);
                sum += (value * value);
            }
        }
        return (float) Math.sqrt(sum);
    }

    /**
     * The conjugate value of this matrix, only applicable if the matrix is 
     * a quaternion.
     * @return a conjugate value of this matrix.
     */
    public IMatrix conjugate() {
        return new Scalar(Float.NaN);
    }

    /**
     * Utility function that creates a new matrix for support of Hadamard functions.
     * The new matrix will have a maximum number of rows of the two provided matrices,
     * and the maximum number of columns.
     * @param op1 the first matrix.
     * @param op2 the second matrix.
     * @return a new matrix with dimension [max(op1.row,op2.row),max(op1.column, op2.column)]
     */
    static IMatrix maxMatrix(IMatrix op1, IMatrix op2) {
        int rows = Math.max(
                op1 != null ? op1.rows : 0,
                op2 != null ? op2.rows : 0
        );
        int columns = Math.max(
                op1 != null ? op1.columns : 0,
                op2 != null ? op2.columns : 0
        );
        if (rows == 1) {
            switch (columns) {
                case 1:
                    return new Scalar();
                case 2:
                    return new Vector2D();
                case 3:
                    return new Vector3D();
                case 4:
                    return new Quaternion();
                default:
                    return new Scalar(Float.NaN);
            }
        } else {
            return new Scalar(Float.NaN);
        }
    }

    /**
     * Utility function that creates a new matrix for support of for example magnitude function,
     * for example calculate the magnitude of a matrix where vectors are stored in rows, and
     * the calculated magnitude is stored into a matrix with the same number of rows and one column.
     * The new matrix will have a maximum number of rows of the two provided matrices,
     * and a number of columns of one.
     * @param op1 the first matrix.
     * @param op2 the second matrix.
     * @return a new matrix with dimension [max(op1.row,op2.row),1]
     */
    static IMatrix maxRowMatrix(IMatrix op1, IMatrix op2) {
        int rows = Math.max(
                op1 != null ? op1.rows : 0,
                op2 != null ? op2.rows : 0
        );
        int columns = 1;
        if (rows == 1) {
            switch (columns) {
                case 1:
                    return new Scalar();
                case 2:
                    return new Vector2D();
                case 3:
                    return new Vector3D();
                case 4:
                    return new Quaternion();
                default:
                    return new Scalar(Float.NaN);
            }
        } else {
            return new Scalar(Float.NaN);
        }
    }

    /**
     * Utility function that creates a new matrix on the basis of the columns
     * of the two matrix parameters.
     * @param op1 the first matrix.
     * @param op2 the second matrix.
     * @return a new matrix with dimension [1,max(op1.columns,op2.columns)]
     */
    static IMatrix maxColumnMatrix(IMatrix op1, IMatrix op2) {
        int rows = 1;
        int columns = Math.max(
                op1 != null ? op1.columns : 0,
                op2 != null ? op2.columns : 0
        );
        if (rows == 1) {
            switch (columns) {
                case 1:
                    return new Scalar();
                case 2:
                    return new Vector2D();
                case 3:
                    return new Vector3D();
                case 4:
                    return new Quaternion();
                default:
                    return new Scalar(Float.NaN);
            }
        } else {
            return new Scalar(Float.NaN);
        }
    }

    /**
     * Applies a binary operation component wise operation on the op1 and op2 
     * parameters. The BinaryOp parameter can be provided as a lambda.
     * @param op1 the first operand matrix for the binary operation.
     * @param op2 the second operand matrix for the binary operation.
     * @param func the function to apply.
     * @param result the matrix where the result of the operation will be stored.
     */
    static void binaryOp(IMatrix op1, IMatrix op2, BinaryOp func, IMatrix result) {
        for (int r = 0; r < result.rows; ++r) {
            for (int c = 0; c < result.columns; ++c) {
                float x1 = op1.get(r, c);
                float x2 = op2.get(r, c);
                float fr = func.calc(x1, x2);
                result.set(r, c, fr);
            }
        }
    }

    /**
     * Applies an unary operation on the op1 matrix and stores it into the result matrix.
     * @param op1 the first operand matrix for the binary operation.
     * @param func the function to apply.
     * @param result the matrix where the result of the operation will be stored.
     */
    static void unaryOp(IMatrix op1, UnaryOp func, IMatrix result) {
        for (int r = 0; r < result.rows; ++r) {
            for (int c = 0; c < result.columns; ++c) {
                float x1 = op1.get(r, c);
                float fr = func.calc(x1);
                result.set(r, c, fr);
            }
        }
    }

    /**
     * Applies a component wise binary operation on two matrices and adds
     * the results together.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @param func the binary function to apply.
     * @param result the matrix to store the result in.
     */
    static void reduceToSum(IMatrix op1, IMatrix op2, BinaryOp func, IMatrix result) {
        int maxc = Math.max(op1.columns, op2.columns);
        for (int r = 0; r < result.rows; ++r) {
            float sum = 0;
            for (int c = 0; c < maxc; ++c) {
                float x1 = op1.get(r, c);
                float x2 = op2.get(r, c);
                float fr = func.calc(x1, x2);
                sum += fr;
            }
            result.set(r, 0, sum);
        }
    }

    /**
     * Convenience function which calculates a new matrix that is the
     * negative version of the provided matrix.
     * @param op1 the matrix to negate.
     * @return a new matrix with the negated values of the first matrix.
     */
    static IMatrix neg(IMatrix op1) {
        IMatrix result = maxMatrix(op1, null);
        IMatrix.unaryOp(op1, (float x1) -> {
            return -x1;
        }, result);
        return result;
    }

    /**
     * Convenience function to add two matrices together. The dimension of
     * the resulting matrix is determined by the maxMatrix function.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return the resulting (new) matrix.
     */
    static IMatrix add(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 + x2;
        }, result);
        return result;
    }

    /**
     * Convenience function to subtract two matrices. The dimension of
     * the resulting matrix is determined by the maxMatrix function.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return the resulting (new) matrix.
     */
    static IMatrix subtract(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 - x2;
        }, result);
        return result;
    }

    /**
     * Convenience function to multiply two matrices. The dimension of
     * the resulting matrix is determined by the maxMatrix function.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return the resulting (new) matrix.
     */
    static IMatrix mul(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 * x2;
        }, result);
        return result;
    }

    /**
     * Convenience function to multiply two quaternions together.
     * @param op1 the first quaternion operand.
     * @param op2 the second quaternion operand.
     * @return the resulting (new) quaternion.
     */
    static IMatrix quatMul(IMatrix op1, IMatrix op2) {
        float x1 = op1.get(0, 0);
        float y1 = op1.get(0, 1);
        float z1 = op1.get(0, 2);
        float w1 = op1.get(0, 3);

        float x2 = op2.get(0, 0);
        float y2 = op2.get(0, 1);
        float z2 = op2.get(0, 2);
        float w2 = op2.get(0, 3);

        float qX = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
        float qY = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
        float qZ = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
        float qW = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
        Quaternion result = new Quaternion(qX, qY, qZ, qW);
        return result;
    }
    
    /**
     * Convenience function to divide two matrices. The dimension of
     * the resulting matrix is determined by the maxMatrix function.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return the resulting (new) matrix.
     */
    static IMatrix divide(IMatrix  op1, IMatrix op2){
	IMatrix result = maxMatrix(op1, op2);
	IMatrix.binaryOp(op1, op2, (float x1, float x2)-> {return x1 / x2; }, result);
	return result;
    }

    /**
     * The dot function is a contextual operation. For two vectors, this is
     * the matrix multiplication with the transposed second operand. For quaternions,
     * this is the quaternion operation.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return the resulting matrix.
     */
    static IMatrix dot(IMatrix op1, IMatrix op2){
        if ((op1.type == Type.VECTOR || op1.type == Type.MATRIX)
		&& (op2.type == Type.VECTOR || op2.type == Type.MATRIX))
	{
		IMatrix result = maxRowMatrix(op1, op2);
		reduceToSum(op1, op2, (float x1, float x2)-> {return x1 * x2; }, result);
		return result;
	}
	else {
		return quatMul(op1, op2);
	}
    }
	
    /**
     * Raises all the elements of the matrix to a power.
     * @param op1 the first matrix operand.
     * @param op2 the second matrix operand.
     * @return a new matrix with all the elements raised to the power.
     */
    static IMatrix power(IMatrix op1, IMatrix op2){
        float power = op2.get(0, 0);
	IMatrix result = maxMatrix(op1, null);
	IMatrix.unaryOp(op1, (var x1) -> {return (float)Math.pow(x1,power); }, result);
	return result;
    }

    /**
     * Checks if the cell defined by (ri,ci) is in the range of this matrix.
     * @param ri the row index.
     * @param ci the column index.
     * @return true if the cell is in range, false otherwise.
     */
    protected boolean inRange(int ri, int ci) {
        return (ri >= 0 && ri < rows && ci >= 0 && ci < columns);
    }

}
