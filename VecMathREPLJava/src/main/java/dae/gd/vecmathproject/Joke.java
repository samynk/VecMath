package dae.gd.vecmathproject;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Koen.Samyn
 */
public class Joke {

    private List<String> lines = new ArrayList<>();

    public Joke(String line) {
        lines.add(line);
    }

    public Joke(String line1, String line2) {
        lines.add(line1);
        lines.add(line2);
    }

    void printJoke(VecMathListener listener) {
        if (lines.size() >= 1) {
            listener.printInfo(lines.get(0), true);
        }
        if (lines.size() >= 2) {
            listener.printInfo(lines.get(1), true);
        }
    }

}
