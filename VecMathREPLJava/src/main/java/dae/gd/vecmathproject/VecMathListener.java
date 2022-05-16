package dae.gd.vecmathproject;

import dae.gd.VecMathLexer;
import dae.gd.VecMathParser;
import dae.gd.VecMathParserBaseListener;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Stack;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ErrorNode;

/**
 *
 * @author Koen.Samyn
 */
public class VecMathListener extends VecMathParserBaseListener {

    private HashMap<String, IMatrix> constants = new HashMap<>();
    private HashMap<String, IMatrix> varMap = new HashMap<>();

    private Stack<IMatrix> exprStack = new Stack();
    private boolean exit = false;
    private static final double RADTODEG= 180/Math.PI;
    private static final double DEGTORAD= Math.PI/180;
    private int decimalPlaces = 6;
    private String floatFormat = "%." + decimalPlaces + "f";

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    private boolean errorFlagged = false;
    private String currentCodeLine;
    private String helpString;

    private JokeGenerator jokeGenerator = new JokeGenerator();

    /**
     * Creates a new VecMathListener object with a number of constants.
     */
    VecMathListener() {
        float piVal = (float) Math.PI;
        constants.put("Pi", new Scalar(piVal));
        constants.put("PI", new Scalar(piVal));
        constants.put("pi", new Scalar(piVal));
        constants.put("e", new Scalar((float) Math.E));
        readHelp();
    }

    /**
     * Prints a prompt text.
     *
     * @param text the text to print.
     */
    public void prompt(String text) {
        ConsolePrint.printPrompt(text);
        errorFlagged = false;
        exprStack.clear();
    }

    private void readHelp() {
        try {
            InputStream in = this.getClass().getClassLoader()
                    .getResourceAsStream("help.txt");
            helpString = new String(in.readAllBytes(), StandardCharsets.UTF_8);
        } catch (IOException ex) {
            Logger.getLogger(VecMathListener.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Sets the current code line.
     *
     * @param codeLine the code line to sets.
     */
    public void setCurrentCodeLine(String codeLine) {
        this.currentCodeLine = codeLine;
    }

    public void exec(String code) {
        VecMathLexer vmLexer = new VecMathLexer(CharStreams.fromString(code));
        setCurrentCodeLine(code);
        CommonTokenStream stream = new CommonTokenStream(vmLexer);

        VecMathParser parser = new VecMathParser(stream);
        parser.addParseListener(this);
        parser.expression();
    }

    public IMatrix getVariable(String id) {
        if (varMap.containsKey(id)) {
            return varMap.get(id);
        } else {
            return null;
        }
    }

    /**
     * Creates a newline on the console.
     */
    public void newline() {
        System.out.print("\n");
    }

    /**
     * Exit of the command rule.
     *
     * @param ctx the Command Context
     */
    public void exitCommand(VecMathParser.CommandContext ctx) {
        if (ctx.EXIT() != null) {
            exit = true;
        } else if (ctx.PRINTALL() != null) {
            for (var pair : varMap.entrySet()) {
                prompt(pair.getKey());
                prompt(" = ");
                pair.getValue().print(floatFormat);
                newline();
            }
        } else if (ctx.JOKE() != null) {
            jokeGenerator.printRandomJoke(this);
        } else if (ctx.HELP() != null) {
            printMarkDown(helpString);
        }
    }

    void printMarkDown(String text) {
        if (text == null || text.isBlank()) {
            return;
        }
        String lines[] = text.split("\\r?\\n");
        for (String line : lines) {
            if (!line.isEmpty() && line.charAt(0) == '#') {
                printInfo(line.substring(1), true);
            } else {
                printText(line, true);
            }
        }
    }

    public void exitAssign(VecMathParser.AssignContext ctx) {
        var idToken = ctx.ID();
        var assignToken = ctx.ASSIGN();
        if (idToken != null && assignToken != null) {
            String varId = ctx.ID().getText();
            if (stackIsValid()) {
                varMap.put(varId, popFromStack());
                if (ctx.SEMI() == null) {
                    printVariable(varId);
                }
            } else {
                printError("I do not know " + varId, true);
                printInfo("If you meant this as a command, this is a command I do not understand.", true);
                printInfo("Type 'help' for a list of commands.", true);
                printInfo("If you want this to be a variable you need the following format: b = 7", true);
            }
        } else if (ctx.value() != null) {
            IMatrix result = popFromStack();
            result.print(floatFormat);
        } else {
            printError("Assigning a new variable should be done with the equals sign.", true);
            printInfo("For example : a = 10", true);
        }
    }

    /**
     * Checks if the stack is valid for assignment (size equal to 1).
     *
     * @return true if the stack is valid for assignment, false otherwise.
     */
    private boolean stackIsValid() {
        return this.exprStack.size() == 1;
    }

    /**
     * Checks if the stack size has the required minimum of elements.
     *
     * @param size the size to check
     * @return true if the stack has the minimum required stack size.
     */
    private boolean checkStackSize(int size) {
        return exprStack.size() >= size;
    }

    public void exitPrint(VecMathParser.PrintContext ctx) {
        if (ctx.ID() != null) {
            String id = ctx.ID().getText();
            printVariable(id);
        } else {
            printError("Use the print statement with a variable id, for example: 'print var'", true);
        }
    }

    private void printVariable(String id) {
        if (varMap.containsKey(id)) {
            printInfo(id, false);
            printInfo(" = ", false);
            varMap.get(id).print(floatFormat);
            newline();
        } else {
            printError("Could not find variable " + id + ",are you sure it exists?", true);
            printInfo("Use the 'printAll' command to see the current list of variables.", true);

        }
    }

    @Override
    public void exitPrecision(VecMathParser.PrecisionContext ctx) {
        if (ctx.FLOAT() != null) {
            float p = Float.parseFloat(ctx.FLOAT().getText());
            int precision = Math.round(p);
            if (precision >= 0) {
                decimalPlaces = precision;
                floatFormat = "%." + decimalPlaces + "f";
            } else {
                printInfo("The precision must be a positive number, specifying the number of decimal places.", true);
                printInfo("For example: precision 4", true);
            }
        } else {
            printInfo("Specify the number of decimal places for this shell.", true);
            printInfo("For example : precision 3", true);
        }
    }

    @Override
    public void exitClear(VecMathParser.ClearContext ctx) {
        if (ctx.ID() != null) {
            String id = ctx.ID().getText();
            varMap.remove(id);
        } else {
            varMap.clear();
        }
    }

    public void exitLiteral(VecMathParser.LiteralContext ctx) {
        if (ctx.FLOAT() != null) {
            float x = Float.parseFloat(ctx.FLOAT().getText());
            Scalar s = new Scalar(x);
            exprStack.push(s);
        }
    }

    public void exitVector(VecMathParser.VectorContext ctx) {
        int dim = ctx.value().size();
        switch (dim) {
            case 1: {
                IMatrix f = popFromStack();
                Scalar s = new Scalar(f.get(0, 0));
                exprStack.push(s);
                break;
            }
            case 2: {

                IMatrix y = popFromStack();
                IMatrix x = popFromStack();
                Vector2D v2d = new Vector2D(x.get(0, 0), y.get(0, 0));
                exprStack.push(v2d);
                break;
            }
            case 3: {
                IMatrix z = popFromStack();
                IMatrix y = popFromStack();
                IMatrix x = popFromStack();
                Vector3D v3d = new Vector3D(x.get(0, 0), y.get(0, 0), z.get(0, 0));
                exprStack.push(v3d);
                break;
            }
        }
    }

    @Override
    public void exitComplex(VecMathParser.ComplexContext ctx) {
        if (checkStackSize(2)) {
            IMatrix i = popFromStack();
            IMatrix r = popFromStack();
            Complex c = new Complex(r.get(0, 0), i.get(0, 0));
            exprStack.push(c);
        }
    }

    @Override
    public void exitQuaternion(VecMathParser.QuaternionContext ctx) {
        IMatrix z = popFromStack();
        IMatrix y = popFromStack();
        IMatrix x = popFromStack();
        IMatrix w = popFromStack();
        Quaternion q = new Quaternion(x.get(0, 0), y.get(0, 0), z.get(0, 0), w.get(0, 0));
        exprStack.push(q);
    }

    @Override
    public void exitValue(VecMathParser.ValueContext ctx) {
        if (ctx.ID() != null) {
            String id = ctx.ID().getText();
            if (varMap.containsKey(id)) {
                exprStack.push(varMap.get(id));
            } else if (constants.containsKey(id)) {
                exprStack.push(constants.get(id));
            } else {
                // yeah this is an error , what to do
                exprStack.push(new Scalar(0));
            }
        } else if (ctx.op != null) {

            switch (ctx.op.getType()) {
                case VecMathLexer.ABS: {
                    IMatrix op2 = popFromStack();
                    if (op2 != null) {
                        Scalar s = new Scalar(op2.magnitude());
                        exprStack.push(s);
                    } else {
                        printError("An error occurred, maybe you forgot to asssign the value to a variable?", true);
                    }
                    break;
                }

                case VecMathLexer.ADD: {
                    // unary case: do nothing leave value on stack.
                    if (ctx.op2 != null && checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.add(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.MINUS: {
                    // can be unary minus!
                    if (ctx.op2 == null && checkStackSize(1)) {
                        IMatrix op = popFromStack();
                        exprStack.push(IMatrix.neg(op));
                    } else if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.subtract(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.MUL: {
                    if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.mul(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.CROSS: {
                    if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.cross(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.POWER: {
                    if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.power(op1, op2));
                    }
                    break;
                }

                case VecMathLexer.DIV: {
                    if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.divide(op1, op2));
                    }
                    break;
                }
                case VecMathLexer.DOT: {
                    if (checkStackSize(2)) {
                        IMatrix op2 = popFromStack();
                        IMatrix op1 = popFromStack();
                        exprStack.push(IMatrix.dot(op1, op2));
                    }
                    break;
                }
            }
        } else if (ctx.f != null && checkStackSize(1)) {
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
                if ("abs".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return Math.abs(x);
                    }, result);
                } else if ("norm".equals(funcName)) {
                    result = new Scalar(op.magnitude());
                } else if ("inv".equals(funcName)) {
                    result = op.inverse();
                } else if ("sind".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)Math.sin(x * DEGTORAD);
                    }, result);
                } else if ("cosd".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)Math.cos(x * DEGTORAD);
                    }, result);
                } else if ("tand".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)Math.tan(x * DEGTORAD);
                    }, result);
                } else if ("asind".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)(RADTODEG * Math.asin(x));
                    }, result);
                } else if ("acosd".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)(RADTODEG * Math.acos(x));
                    }, result);
                } else if ("atand".equals(funcName)) {
                    result = IMatrix.maxMatrix(op, null);
                    IMatrix.unaryOp(op, (float x) -> {
                        return (float)(RADTODEG * Math.atan(x));
                    }, result);

                } else {
                    printError("There is no function called " + funcName + " (yet)!", true);
                    printInfo("Maybe you made a typo? (type 'help' for a list of functions)", true);
                    errorFlagged = true;
                }
            }
            pushToStack(result);
        }
    }

    /**
     * Remove an element from the stack and return it.
     *
     * @return the top value of the stacks.
     */
    private IMatrix popFromStack() {
        if (exprStack.size() > 0) {
            IMatrix r = exprStack.pop();
            return r;
        } else {
            return new Scalar(Float.NaN);
        }
    }

    /**
     * Add value to the stack.
     *
     * @param toPush the value to push.
     */
    void pushToStack(IMatrix toPush) {
        exprStack.push(toPush);
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

    void printInfo(String text, boolean newLine) {
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
