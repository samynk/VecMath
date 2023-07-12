#include <iostream>
#include "VecMathLexer.h"
#include "VecMathParser.h"
#include "VecMathListener.h"
#include "Vocabulary.h"
#include "ANTLRInputStream.h"


#ifdef _WIN32
#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif 

VecMathListener listener{};
std::stringstream redirectStream;
std::string lastOutput;
extern "C" {
	const char* prompt(char* input)
	{
		redirectStream.str(std::string());
		listener.exec(input);
		lastOutput = redirectStream.str();
		std::cerr << lastOutput;
		return lastOutput.c_str();
	}
}

int main()
{
#ifdef _WIN32
#ifndef NDEBUG
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
#endif
	std::string codeLine;
	size_t tokenType = -1;
	using namespace antlr4;
	using namespace VecMath;
	{
		try {
			

#ifdef __EMSCRIPTEN__
			std::cout.rdbuf(redirectStream.rdbuf());
			listener.setUseColors(false);
#endif
		}
		catch (std::system_error const& e) {
			std::cerr << e.what() << std::endl;
			std::cerr << e.code() << std::endl;
		}
	}
#ifdef _WIN32
#ifndef NDEBUG
	_CrtDumpMemoryLeaks();
#endif
#endif
}