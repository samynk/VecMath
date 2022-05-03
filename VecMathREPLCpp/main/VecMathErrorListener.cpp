#include "VecMathErrorListener.h"
#include <iostream>

void VecMathErrorListener::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
	//std::cout << "Error detected\n";
}

void VecMathErrorListener::reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet& ambigAlts, antlr4::atn::ATNConfigSet* configs)
{
	std::cout << "Ambiguity\n";
}

void VecMathErrorListener::reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet& conflictingAlts, antlr4::atn::ATNConfigSet* configs)
{
	std::cout << "Full context attempt\n";
}

void VecMathErrorListener::reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet* configs)
{
	std::cout << "Context sensitivity report\n";
}
