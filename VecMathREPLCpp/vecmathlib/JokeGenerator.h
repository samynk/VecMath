#pragma once
#include "Joke.h"
#include "Console.h"
#include <vector>
#include <random>
class JokeGenerator
{
public:
	JokeGenerator();
	void printRandomJoke(const Console& consoleHandle) ;
private:
	std::vector<Joke> m_Jokes;
	std::default_random_engine m_Generator; // seed the generator
};
