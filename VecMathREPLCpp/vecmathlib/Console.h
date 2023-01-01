#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>

/**
 * \brief class for writing to the console in a platform independent way
 * \author Koen Samyn
 */
class Console
{
public:
	Console();
	void Print(const unsigned short color, const std::string& text) const;
	void Print(const unsigned short color, const float value) const;
	void SetPrecision(const int precision);
	
	void SetColor(const unsigned short color) const;
	void Print(char singleCharacter) const;
	void Print(const std::string& text) const;
	void Reset();
	void ClearScreen() const;
	void NewLine() const;

#ifdef _WIN32
	
	static constexpr unsigned short VMF_GREEN = 2;
	static constexpr unsigned short VMF_BRIGHTGREEN = 10;
	static constexpr unsigned short VMF_CYAN = 3;
	static constexpr unsigned short VMF_BRIGHTCYAN = 11;
	static constexpr unsigned short VMF_RED = 4;
	static constexpr unsigned short VMF_BRIGHTRED = 12;
	static constexpr unsigned short VMF_LIGHTGRAY = 7;
	static constexpr unsigned short VMF_GRAY = 8;
	static constexpr unsigned short VMF_WHITE = 15;
#else
	static constexpr unsigned short VMF_GREEN = 32;
	static constexpr unsigned short VMF_BRIGHTGREEN = 92;
	static constexpr unsigned short VMF_CYAN = 36;
	static constexpr unsigned short VMF_BRIGHTCYAN = 96;
	static constexpr unsigned short VMF_RED = 31;
	static constexpr unsigned short VMF_BRIGHTRED = 91;
	static constexpr unsigned short VMF_LIGHTGRAY = 37;
	static constexpr unsigned short VMF_GRAY = 90;
	static constexpr unsigned short VMF_WHITE = 97;
#endif
private:
	int m_Precision{5};
#ifdef _WIN32
	HANDLE m_ConsoleHandle;
#endif
};
