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
void printTokens(
	antlr4::CommonTokenStream& stream,
	const antlr4::dfa::Vocabulary& symbols);

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
			VecMathListener listener{};
			listener.promptHeader();
			do {
				listener.prompt("vecmath>");
				std::getline(std::cin, codeLine);
				listener.exec(codeLine);
			} while (!listener.isExit());
			listener.clearVariables();
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

void printTokens(
	antlr4::CommonTokenStream& stream,
	const antlr4::dfa::Vocabulary& symbols
) {
	using namespace std;
	for (auto token : stream.getTokens()) {
		cout << token->getText();
		cout << " [";
		cout << symbols.getSymbolicName(token->getType());
		cout << "] ";
	}
	cout << endl;
}