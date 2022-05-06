package dae.gd.vecmathproject;

/**
 *
 * @author Koen.Samyn
 */
public abstract class IMatrix {

    private int rows;
    private int columns;
    protected Type type = Type.VECTOR;
    protected AddressMode addressMode = AddressMode.CLAMP;
    protected float borderValue = 0;

    public enum Type {
        MATRIX, VECTOR, QUATERNION
    };

    public enum AddressMode {
        CLAMP, BORDER, BOUNDSERROR
    };

    IMatrix(Type type, AddressMode mode, int rows, int columns) {
        this(type,mode,0,rows,columns);
    }

    IMatrix(Type type, AddressMode mode, float borderValue, int rows, int columns) {
        this.type = type;
        this.addressMode = mode;
        this.borderValue = borderValue;
        this.rows = rows;
        this.columns = columns;
    }

    public abstract float get(int ri, int ci);

    public abstract void set(int ri, int ci, float value);

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

    public IMatrix conjugate() {
        return new Scalar(Float.NaN);
    }

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

    static void unaryOp(IMatrix op1, UnaryOp func, IMatrix result) {
        for (int r = 0; r < result.rows; ++r) {
            for (int c = 0; c < result.columns; ++c) {
                float x1 = op1.get(r, c);
                float fr = func.calc(x1);
                result.set(r, c, fr);
            }
        }
    }

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

    static IMatrix neg(IMatrix op1) {
        IMatrix result = maxMatrix(op1, null);
        IMatrix.unaryOp(op1, (float x1) -> {
            return -x1;
        }, result);
        return result;
    }

    static IMatrix add(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 + x2;
        }, result);
        return result;
    }

    static IMatrix subtract(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 - x2;
        }, result);
        return result;
    }

    static IMatrix mul(IMatrix op1, IMatrix op2) {
        IMatrix result = maxMatrix(op1, op2);
        IMatrix.binaryOp(op1, op2, (float x1, float x2) -> {
            return x1 * x2;
        }, result);
        return result;
    }

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
    static IMatrix divide(IMatrix  op1, IMatrix op2){
	IMatrix result = maxMatrix(op1, op2);
	IMatrix.binaryOp(op1, op2, (float x1, float x2)-> {return x1 / x2; }, result);
	return result;
    }

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
	
    static IMatrix power(IMatrix op1, IMatrix op2){
        float power = op2.get(0, 0);
	IMatrix result = maxMatrix(op1, null);
	IMatrix.unaryOp(op1, (var x1) -> {return (float)Math.pow(x1,power); }, result);
	return result;
    }

    protected boolean inRange(int ri, int ci) {
        return (ri >= 0 && ri < rows && ci >= 0 && ci < columns);
    }

}
