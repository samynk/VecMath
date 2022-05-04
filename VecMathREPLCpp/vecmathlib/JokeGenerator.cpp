#include "JokeGenerator.h"
#include <fstream>
#include <random>

JokeGenerator::JokeGenerator()
{
	std::ifstream jokeFile{ "resources/jokes.txt" };
	if (jokeFile.is_open()) {
		while (!jokeFile.eof()) {
			std::string line1,line2;
			std::getline(jokeFile, line1);
			if (line1[0] == 'Q' && !jokeFile.eof()) {
				// read another line
				std::getline(jokeFile, line2);
				Joke joke{ line1, line2 };
				m_Jokes.push_back(joke);
			}
			else if (line1[0] == 'S') {
				Joke joke{ line1};
				m_Jokes.push_back(joke);
			}
		}
	}
}

void JokeGenerator::printRandomJoke(HANDLE consoleHandle)
{
	std::uniform_int_distribution<std::size_t> distribution(0,m_Jokes.size()-1);
	std::size_t number = distribution(m_Generator);
	m_Jokes[number].printJoke(consoleHandle);
}
