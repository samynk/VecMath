package dae.gd.vecmathproject;

import dae.gd.VecMathLexer;
import dae.gd.VecMathParser;
import dae.gd.VecMathParserBaseListener;
import java.util.HashMap;
import java.util.Stack;
import org.antlr.v4.runtime.tree.ErrorNode;

/**
 *
 * @author Koen.Samyn
 */
public class VecMathListener extends VecMathParserBaseListener {

    private HashMap<String, IMatrix> constants = new HashMap<>();
    private HashMap<String, IMatrix> variables = new HashMap<>();

    private Stack<IMatrix> calculationStack = new Stack();
    private boolean exit = false;

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    private boolean errorFlagged = false;
    private String currentCodeLine;

    VecMathListener() {
        float piVal = (float) Math.PI;
        constants.put("Pi", new Scalar(piVal));
        constants.put("PI", new Scalar(piVal));
        constants.put("pi", new Scalar(piVal));
        constants.put("e", new Scalar((float) Math.E));
    }

    public void prompt(String text) {
        ConsolePrint.printPrompt(text);
        errorFlagged = false;
    }

    public void setCurrentCodeLine(String codeLine) {
        this.currentCodeLine = codeLine;
    }

    public void newline() {
        System.out.print("\n");
    }

    public void exitCommand(VecMathParser.CommandContext ctx) {
        if (ctx.EXIT() != null) {
            exit = true;
        } else if (ctx.PRINTALL() != null) {
            for (var pair : variables.entrySet()) {
                prompt(pair.getKey());
                prompt(" = ");
                pair.getValue().print();
                newline();
            }
        } else if (ctx.CLEAR() != null) {
            variables.clear();
        } else if (ctx.HELP() != null) {
            printInfo("your friendly neighbourhood help",true);

            printInfo("Available commands",true);
            printText("help : prints this very helpful message.",true);
            printText("print variableName: prints the value of the variable name to output.",true);
            printText("printAll : prints the values of all the current variables to output.",true);
            printText("exit or bye : stops this fine REPL shell.",true);
            printInfo("Variables",true);
            printText("a=1.0 -> creates a scalar value",true);
            printText("b=[1.0] -> creates a scalar value (a one dimensional vector).",true);
            printText("c=[2,3] -> creates a two dimensional vector.",true);
            printText("d=[2,3,9] -> creates a three dimensional vector.",true);
            printText("q=[3,(2,3,9)] -> creates a quaternion with i=2, j=3, k=9 and w=3.",true);
            printInfo("Operators",true);
            printText("* : component wise multiplication.",true);
            printText("/ : component wise division.",true);
            printText("+ : component wise addition.",true);
            printText("- : component wise subtraction, unary negation.",true);
            printText("^ : raise vector, matrix or quaternion to a power.",true);
            printText(". : dot product for vectors, complex multiplication for quaternions.",true);
            printInfo("Mathematical functions",true);
            printText("sin, cos, tan: sine, cosine and tangent function of a value in radians. These functions will be applied on all components.",true);
            printText("asin, acos, atan: arc sine, arc cosine and arc tangent function. Returns an angle in radians. These functions will be applied on all components.",true);
            printText("con : calculates the conjugate of a quaternion.",true);
            printText("rad2deg : converts a value in radians to degrees.",true);
            printText("deg2rad : converts a value in degrees to radians.",true);
            printInfo("Here ends the shift of the friendly neighbourhood help.",true);
        }
    }

    public void exitAssign(VecMathParser.AssignContext ctx) {
        if (ctx.ID() != null) {
            String varId = ctx.ID().getText();
            if (stackIsValid()) {
                variables.put(varId, popFromStack());
            } else {
                printError("I do not know '" + varId ,true);
                printInfo("If you meant this as a command, this is a command I do not understand.",true);
                printInfo("Type 'help' for a list of commands.",true);
                printInfo("If you want this to be a variable you need the following format: b = 7",true);
            }
        } else if (ctx.ASSIGN() == null) {
            printError("Assigning a new variable should be done with the equals sign.",true);
            printInfo("For example : a = 10",true);
        }
    }

    private boolean stackIsValid() {
        return this.calculationStack.size() == 1;
    }

    public void exitPrint(VecMathParser.PrintContext ctx) {
        String id = ctx.ID().getText();
        if (variables.containsKey(id)) {
            printInfo(id, false);
            printText(" = ", false);
            variables.get(id).print();
            newline();
        }
    }

    public void exitLiteral(VecMathParser.LiteralContext ctx) {
        if (ctx.FLOAT() != null) {
            float x = Float.parseFloat(ctx.FLOAT().getText());
            Scalar s = new Scalar(x);
            calculationStack.push(s);
        }
    }

    public void exitVector(VecMathParser.VectorContext ctx) {
        int dim = ctx.value().size();
        switch (dim) {
            case 1: {
                IMatrix f = popFromStack();
                Scalar s = new Scalar(f.get(0, 0));
                calculationStack.push(s);
                break;
            }
            case 2: {

                IMatrix y = popFromStack();
                IMatrix x = popFromStack();
                Vector2D v2d = new Vector2D(x.get(0, 0), y.get(0, 0));
                calculationStack.push(v2d);
                break;
            }
            case 3: {
                IMatrix z = popFromStack();
                IMatrix y = popFromStack();
                IMatrix x = popFromStack();
                Vector3D v3d = new Vector3D(x.get(0, 0), y.get(0, 0), z.get(0, 0));
                calculationStack.push(v3d);
                break;
            }
        }
    }

    public void exitQuaternion(VecMathParser.QuaternionContext ctx) {
        IMatrix z = popFromStack();
        IMatrix y = popFromStack();
        IMatrix x = popFromStack();
        IMatrix w = popFromStack();
        Quaternion q = new Quaternion(x.get(0, 0), y.get(0, 0), z.get(0, 0), w.get(0, 0));
        calculationStack.push(q);
    }

    public void exitValue(VecMathParser.ValueContext ctx) {
        if (ctx.ID() != null) {
            String id = ctx.ID().getText();
            if (variables.containsKey(id)) {
                calculationStack.push(variables.get(id));
            } else if (constants.containsKey(id)) {
                calculationStack.push(constants.get(id));
            } else {
                // yeah this is an error , what to do
                calculationStack.push(new Scalar(0));
            }
        } else if (ctx.op != null) {

            switch (ctx.op.getType()) {
                case VecMathLexer.ABS: {
                    IMatrix op2 = popFromStack();
                    Scalar s = new Scalar(op2.magnitude());
                    calculationStack.push(s);
                    break;
                }

                case VecMathLexer.ADD: {
                    // unary case: do nothing leave value on stack.
                    if (ctx.op2 != null) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        calculationStack.push(IMatrix.add(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.MINUS: {
                    // can be unary minus!
                    if (ctx.op2 == null) {
                        IMatrix op = popFromStack();
                        calculationStack.push(IMatrix.neg(op));
                    } else {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();

                        calculationStack.push(IMatrix.subtract(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.MUL: {
                    IMatrix op2 = popFromStack();
                    IMatrix op1 = popFromStack();

                    calculationStack.push(IMatrix.mul(op1, op2));
                    break;
                }
                case VecMathLexer.POWER: {
                    IMatrix op2 = popFromStack();
                    IMatrix op1 = popFromStack();

                    calculationStack.push(IMatrix.power(op1, op2));
                    break;
                }
                case VecMathLexer.DIV: {
                    IMatrix op2 = popFromStack();
                    IMatrix op1 = popFromStack();

                    calculationStack.push(IMatrix.divide(op1, op2));
                    break;
                }
                case VecMathLexer.DOT: {
                    IMatrix op2 = popFromStack();
                    IMatrix op1 = popFromStack();

                    calculationStack.push(IMatrix.dot(op1, op2));
                    break;
                }
            }
        } else if (ctx.f != null) {
            IMatrix op = popFromStack();
            IMatrix result = IMatrix.maxMatrix(op, null);
            var ctxFunc = ctx.f;
            if (ctxFunc.COS() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.cos(x);
                }, result);
            } else if (ctxFunc.SIN() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.sin(x);
                }, result);
            } else if (ctxFunc.TAN() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.tan(x);
                }, result);
            } else if (ctxFunc.ACOS() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.acos(x);
                }, result);
            } else if (ctxFunc.ASIN() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.asin(x);
                }, result);
            } else if (ctxFunc.ATAN() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.atan(x);
                }, result);
            } else if (ctxFunc.DEGTORAD() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) (x * Math.PI / 180.0f);
                }, result);
            } else if (ctxFunc.RADTODEG() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) (x * 180 / Math.PI);
                }, result);
            } else if (ctxFunc.SQRT() != null) {
                IMatrix.unaryOp(op, (float x) -> {
                    return (float) Math.sqrt(x);
                }, result);
            } else if (ctxFunc.CON() != null) {
                // only do this if it is a quaternion.
                result = op.conjugate();
            } else if (ctxFunc.ID() != null) {
                String funcName = ctxFunc.ID().getText();
                printError("There is no function called " + funcName + " (yet)!", true);
                printInfo("Maybe you made a typo? (type 'help' for a list of functions)", true);
                errorFlagged = true;
            }
            pushToStack(result);
        }
    }

    private IMatrix popFromStack() {
        if (calculationStack.size() > 0) {
            IMatrix r = calculationStack.pop();
            return r;
        } else {
            return new Scalar(Float.NaN);
        }
    }

    void pushToStack(IMatrix toPush) {
        calculationStack.push(toPush);
    }

    @Override
    public void visitErrorNode(ErrorNode node) {
        if (errorFlagged) {

            return;
        }
        int type = node.getSymbol().getType();
        var interval = node.getSourceInterval();

        switch (type) {
            case VecMathLexer.COMMA:
                printErrorLoc(interval.a, interval.b, currentCodeLine);
                printError("Was not expecting a comma at character " + Integer.toString(interval.a + 1), true);
                printInfo("Did you forget to use [ and ] to define vectors, matrices or quaternions?", true);
                printInfo("Example: v=[2,3.1,7]", true);
                errorFlagged = true;
                break;
            case VecMathLexer.FLOAT:
                printErrorLoc(interval.a, interval.b, currentCodeLine);
                printError("Was not expecting a float at character " + Integer.toString(interval.a + 1), true);
                printInfo("Did you forget to use commas to define vectors, matrices or quaternions?", true);
                printInfo("Example: v=[2,3.1,7]", true);
                errorFlagged = true;
                break;
        }
    }

    private void printError(String text, boolean newLine) {
        ConsolePrint.printError(text);
        if (newLine) {
            System.out.print("\n");
        }
    }

    private void printInfo(String text, boolean newLine) {
        ConsolePrint.printInfo(text);
        if (newLine) {
            System.out.print("\n");
        }
    }

    private void printErrorLoc(int start, int end, String message) {
        for (int pc = 0; pc < message.length(); ++pc) {
            if (pc < start || pc > end) {
                printInfo(Character.toString(message.charAt(pc)), false);
            } else {
                printError(Character.toString(message.charAt(pc)), false);
            }
        }
        System.out.println("\n");
    }

    private void printText(String text, boolean newLine) {
        ConsolePrint.printText(text);
        if (newLine) {
            System.out.print("\n");
        }
    }
}
