#include <iostream>
#include "VecMathLexer.h"
#include "VecMathParser.h"
#include "VecMathListener.h"
#include "Vocabulary.h"
#include "ANTLRInputStream.h"

void printTokens(
	antlr4::CommonTokenStream& stream,
	const antlr4::dfa::Vocabulary& symbols);

int main()
{
	
	std::string codeLine;
	size_t tokenType = -1;
	using namespace antlr4;
	using namespace VecMath;

	VecMathListener listener{};
	listener.promptHeader();
	do {
		listener.prompt("vecmath>");
		std::getline(std::cin, codeLine);
		//std::cout << "You typed : " << codeLine << std::endl;
		listener.setCurrentCodeLine(codeLine);
		tokenType = -1;
		try {
			ANTLRInputStream is{ codeLine };
			VecMath::VecMathLexer lexer{ &is };

			CommonTokenStream stream{ &lexer };
			VecMathParser parser(&stream);
			parser.removeErrorListeners();
			parser.addParseListener(&listener);
			parser.addErrorListener(&listener);
			parser.expression();
			stream.fill();
			//printTokens(stream,lexer.getVocabulary());
			tokenType = stream.getTokens()[0]->getType();
		}
		catch (IllegalArgumentException ex) {
			listener.printError("Illegal character in string, best to use ASCII characters only. ");
		}
	} while (tokenType != VecMathLexer::EXIT);
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