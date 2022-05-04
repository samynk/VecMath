#pragma once
#include "VecMathParserBaseListener.h"
#include <map>
#include <stack>
#include "IMatrix.h"
#include <windows.h>

class VecMathListener: public VecMath::VecMathParserBaseListener
{
public:
	VecMathListener();

	void prompt(const std::string& text);
	bool isExit() {
		return m_Exit;
	}

	bool stackIsValid() {
		return m_ExprStack.size() == 1;
	}

	void setCurrentCodeLine(std::string codeLine);

	void exitCommand(VecMath::VecMathParser::CommandContext* /*ctx*/) override;
	void exitAssign(VecMath::VecMathParser::AssignContext* ctx) override;
	void exitPrint(VecMath::VecMathParser::PrintContext* ctx) override;
	void exitClear(VecMath::VecMathParser::ClearContext* ctx) override;
	void exitLiteral(VecMath::VecMathParser::LiteralContext* ctx) override;
	void exitVector(VecMath::VecMathParser::VectorContext* ctx) override;
	void exitQuaternion(VecMath::VecMathParser::QuaternionContext* ctx)override;
	void exitValue(VecMath::VecMathParser::ValueContext* ctx) override;
	void exitFunction(VecMath::VecMathParser::FunctionContext* ctx)override;
	void visitErrorNode(antlr4::tree::ErrorNode* node) override;

	void printError(const std::string& message);
	void printInfo(const std::string& message);
	void printText(const std::string& message);
	void printErrorLoc(size_t start, size_t end, const std::string& message);

	IMatrix* popFromStack();
	void pushToStack(IMatrix* toPush);
private:
	HANDLE m_ConsoleHandle;
	bool m_Exit{ false };
	bool m_ErrorFlagged{ false };
	std::map<std::string, IMatrix*> m_VarMap;
	std::map<std::string, IMatrix*> m_Constants;
	std::stack<IMatrix*> m_ExprStack;
	std::string m_CurrentCodeLine;


	static const int ERRORCOLOR{ 12 };
	static const int INFOCOLOR{ 10 };
	static const int TEXTCOLOR{ 7 };
};
