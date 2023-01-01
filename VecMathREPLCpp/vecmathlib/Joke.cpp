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

void Joke::printJoke(const Console& console) const
{
	if (!m_Lines.empty()) {
		console.Print(Console::VMF_BRIGHTGREEN, m_Lines[0]);
		console.NewLine();
	}
	if (m_Lines.size() >= 2) {
		console.Print(Console::VMF_BRIGHTCYAN, m_Lines[1]);
		console.NewLine();
	}
}
