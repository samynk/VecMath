package dae.gd.vecmathproject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Koen.Samyn
 */
public class JokeGenerator {

    private List<Joke> jokes = new ArrayList<>();
    private int currentIndex = 0;

    public JokeGenerator() {

        InputStream in = this.getClass().getClassLoader()
                .getResourceAsStream("jokes.txt");
        if (in != null) {
            try {
                BufferedReader br = new BufferedReader(new InputStreamReader(in));
                String line1 = br.readLine();
                while (line1 != null) {

                    if (line1.charAt(0) == 'Q') {
                        // read another line
                        String line2 = br.readLine();
                        Joke joke = new Joke(line1, line2);
                        jokes.add(joke);
                    } else if (line1.charAt(0) == 'S') {
                        Joke joke = new Joke(line1);
                        jokes.add(joke);
                    }
                    line1 = br.readLine();
                }
                Collections.shuffle(jokes, new Random(System.currentTimeMillis()));
            } catch (IOException ex) {
                Logger.getLogger(JokeGenerator.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    public void printRandomJoke(VecMathListener listener) {
        if (jokes.size() > 0) {
            jokes.get(currentIndex).printJoke(listener);
            currentIndex++;
            if (currentIndex >= jokes.size()) {
                Collections.shuffle(jokes, new Random(System.currentTimeMillis()));
                currentIndex = 0;
            }
        }
    }
}
