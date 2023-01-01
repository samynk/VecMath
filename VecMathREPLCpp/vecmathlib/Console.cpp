#include "Console.h"
#include "iostream"
#include <iomanip>

Console::Console()
{
#ifdef _WIN32
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void Console::Print(const unsigned short color, const std::string& text) const
{
#ifdef _WIN32
	SetConsoleTextAttribute(m_ConsoleHandle, color);
	std::cout << text;
#else
	std::cout << "\033[1;";
	std::cout << color;
	std::cout << text;
	std::cout << "\033[0m";
#endif
}

void Console::Print(const unsigned short color, const float value) const
{
#ifdef _WIN32
	SetConsoleTextAttribute(m_ConsoleHandle, color);
	std::cout << value;
#else
	std::cout << "\033[1;";
	std::cout << color;
	std::cout << text;
	std::cout << "\033[0m";
#endif
}

void Console::SetPrecision(const int precision)
{
	m_Precision = precision;
	std::cout << std::fixed << std::showpoint << std::setprecision(precision);
}

void Console::Print(const std::string& text) const
{
	std::cout << text;
}

void Console::Print(char singleCharacter) const
{
	std::cout << singleCharacter;
}

void Console::SetColor(const unsigned short color) const
{
#ifdef _WIN32
	SetConsoleTextAttribute(m_ConsoleHandle, color);
#else
	std::cout << "\033[1;";
	std::cout << color;
#endif
}

void Console::Reset()
{
#ifdef _WIN32
	SetConsoleTextAttribute(m_ConsoleHandle, VMF_WHITE);
#else
	std::cout << "\033[0m";
#endif
}

void Console::ClearScreen() const
{
#ifdef _WIN32
	// source : https://www.cplusplus.com/articles/4z18T05o/
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };
	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(m_ConsoleHandle, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		m_ConsoleHandle,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		m_ConsoleHandle,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(m_ConsoleHandle, homeCoords);
#else
	std::cout << "\033[2J\033[1;1H";
#endif
}

void Console::NewLine() const
{
	std::cout << "\n";
}
