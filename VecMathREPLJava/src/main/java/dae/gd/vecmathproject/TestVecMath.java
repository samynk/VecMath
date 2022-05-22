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
        Scanner in = new Scanner(System.in);
        VecMathListener listener = new VecMathListener();
        listener.promptHeader();
        int token = -1;
        do {
            listener.prompt("vecmath>");
            String line = in.nextLine();
            listener.exec(line);
        } while (token != VecMathLexer.EXIT);
        System.out.println("Now closing the VecMath REPL shell");
        System.out.println("Thanks for using");
    }
}
