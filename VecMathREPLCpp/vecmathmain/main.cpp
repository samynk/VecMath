#include <iostream>
#include "VecMathLexer.h"
#include "VecMathParser.h"
#include "VecMathListener.h"
#include "Vocabulary.h"
#include "ANTLRInputStream.h"



#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

void printTokens(
	antlr4::CommonTokenStream& stream,
	const antlr4::dfa::Vocabulary& symbols);

int main()
{
#ifndef NDEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
	std::string codeLine;
	size_t tokenType = -1;
	using namespace antlr4;
	using namespace VecMath;
	{
		VecMathListener listener{};
		listener.promptHeader();
		do {
			listener.prompt("vecmath>");
			std::getline(std::cin, codeLine);
			listener.exec(codeLine);
		} while (!listener.isExit());
		listener.clearVariables();
	}
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