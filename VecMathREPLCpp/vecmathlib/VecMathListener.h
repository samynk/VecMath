#pragma once
#include "VecMathParserBaseListener.h"
#include "JokeGenerator.h"
#include "IMatrix.h"
#include "Console.h"
#include <map>
#include <stack>

#include <windows.h>
#include <memory>

class VecMathListener : 
	public VecMath::VecMathParserBaseListener,
	public antlr4::ANTLRErrorListener
{
public:
	VecMathListener();

	void prompt(const std::string& text);
	void promptHeader() const;
	void readHelp();
	bool isExit() {
		return m_Exit;
	}

	bool stackIsValid() {
		return m_ExprStack.size() == 1;
	}

	void setCurrentCodeLine(std::string codeLine);
	void exec(std::string code);
	std::shared_ptr<IMatrix> getVariable(const std::string& id)const;

	void exitCommand(VecMath::VecMathParser::CommandContext* /*ctx*/) override;
	void exitAssign(VecMath::VecMathParser::AssignContext* ctx) override;
	void exitPrint(VecMath::VecMathParser::PrintContext* ctx) override;
	void exitClear(VecMath::VecMathParser::ClearContext* ctx) override;
	void exitLiteral(VecMath::VecMathParser::LiteralContext* ctx) override;
	void exitVector(VecMath::VecMathParser::VectorContext* ctx) override;
	void exitComplex(VecMath::VecMathParser::ComplexContext* ctx) override;
	void exitQuaternion(VecMath::VecMathParser::QuaternionContext* ctx)override;
	void exitValue(VecMath::VecMathParser::ValueContext* ctx) override;
	void exitFunction(VecMath::VecMathParser::FunctionContext* ctx)override;
	void exitPrecision(VecMath::VecMathParser::PrecisionContext* ctx)override;
	void visitErrorNode(antlr4::tree::ErrorNode* node) override;

	void printError(const std::string& message) const;
	void printInfo(const std::string& message) const;
	void printText(const std::string& message) const;
	void printVariable(const std::string& id) const;
	void printVariable(const std::string& id, std::shared_ptr<IMatrix> matrix) const;
	void printErrorLoc(size_t start, size_t end, const std::string& message);
	void printMarkDown(const std::string& text);
	void clearVariables();

	// error listener

	void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
		size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override;
	void reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, bool exact,
		const antlrcpp::BitSet& ambigAlts, antlr4::atn::ATNConfigSet* configs) override;
	void reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex,
		const antlrcpp::BitSet& conflictingAlts, antlr4::atn::ATNConfigSet* configs) override;
	virtual void reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex,
		size_t prediction, antlr4::atn::ATNConfigSet* configs) override;

	std::shared_ptr<IMatrix> popFromStack();
	void pushToExprStack(IMatrix* toPush);
	void addToConstantMap(std::string constantName, IMatrix* constant);
private:
	bool m_Exit{ false };
	bool m_ErrorFlagged{ false };
	std::map<std::string, std::shared_ptr<IMatrix>> m_VarMap;
	std::map<std::string, std::shared_ptr<IMatrix>> m_Constants;
	std::stack<std::shared_ptr<IMatrix>> m_ExprStack;
	std::string m_CurrentCodeLine;
	std::string m_HelpString;
	static constexpr float m_DegToRad{static_cast<float>( M_PI / 180 ) };
	static constexpr float m_RadToDeg{ static_cast<float>(180 / M_PI )};

	JokeGenerator m_JokeGenerator;
	Console m_Console;
};
