package dae.gd.vecmathproject;

import dae.gd.VecMathLexer;
import dae.gd.VecMathParser;
import java.util.Scanner;
import org.antlr.v4.runtime.BufferedTokenStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;

/**
 *
 * @author Koen.Samyn
 */
public class TestVecMath {

    public static void main(String[] args) {
        System.out.println("VecMath REPL shell - v1.0.4 (Koen Samyn - 2022)");
        Scanner in = new Scanner(System.in);
        VecMathListener listener = new VecMathListener();
        int token = -1;
        do {
            listener.prompt("vecmath>");
            String line = in.nextLine();
            listener.setCurrentCodeLine(line);
            VecMathLexer vmLexer = new VecMathLexer(CharStreams.fromString(line));
            CommonTokenStream stream = new CommonTokenStream(vmLexer);

            VecMathParser parser = new VecMathParser(stream);
            parser.addParseListener(listener);
            parser.expression();
        } while (token != VecMathLexer.EXIT);
        System.out.println("Now closing the VecMath REPL shell");
        System.out.println("Thanks for using");
    }
}
