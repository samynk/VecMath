#pragma once
#include <vector>
#include <string>
#include "Console.h"
class Joke
{
public:
	Joke(std::string line);
	Joke(std::string line1, std::string line2);

	void printJoke(const Console& console) const;
private:
	std::vector<std::string> m_Lines;
};
