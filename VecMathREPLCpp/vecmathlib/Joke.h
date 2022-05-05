#pragma once
#include <vector>
#include <string>
#include <windows.h>
class Joke
{
public:
	Joke(std::string line);
	Joke(std::string line1, std::string line2);

	void printJoke(HANDLE console);
private:
	std::vector<std::string> m_Lines;
};
