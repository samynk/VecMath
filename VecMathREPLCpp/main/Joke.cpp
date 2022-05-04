#include "Joke.h"
#include <iostream>

Joke::Joke(std::string line)
{
	m_Lines.push_back(line);
}

Joke::Joke(std::string line1, std::string line2)
{
	m_Lines.push_back(line1);
	m_Lines.push_back(line2);
}

void Joke::printJoke(HANDLE console)
{
	if (m_Lines.size() >= 1) {
		SetConsoleTextAttribute(console, 10);
		std::cout << m_Lines[0] << "\n";
	}
	if (m_Lines.size() >= 2) {
		SetConsoleTextAttribute(console, 11);
		std::cout << m_Lines[1] << "\n";
	}
}
