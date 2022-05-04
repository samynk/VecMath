#pragma once
#include "Joke.h"
#include <windows.h>
#include <vector>
#include <random>
class JokeGenerator
{
public:
	JokeGenerator();
	void printRandomJoke(HANDLE consoleHandle);
private:
	std::vector<Joke> m_Jokes;
	std::mt19937 m_Generator{ std::random_device{}() };
	
};
