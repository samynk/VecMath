#include "VecMathListener.h"
#include "Scalar.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Complex.h"
#include "Quaternion.h"
#include "VecMathLexer.h"
#include <iomanip>

#undef ERROR

VecMathListener::VecMathListener()
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	float fPI = static_cast<float>(M_PI);
	m_Constants["Pi"] = new Scalar(fPI);
	m_Constants["PI"] = new Scalar(fPI);
	m_Constants["pi"] = new Scalar(fPI);
	m_Constants["e"] = new Scalar(static_cast<float>(M_E));
	readHelp();
}

void VecMathListener::prompt(const std::string& text)
{
	SetConsoleTextAttribute(m_ConsoleHandle, 3);
	std::cout << text;
	SetConsoleTextAttribute(m_ConsoleHandle, 8);
	m_ErrorFlagged = false;
	while (!m_ExprStack.empty()) {
		m_ExprStack.pop();
	}
}

void VecMathListener::promptHeader() const
{
	printInfo("VecMath REPL shell 1.0.6 - Koen Samyn - 2022");
	printInfo("please report issues at https://github.com/samynk/VecMath");
}

void VecMathListener::readHelp()
{
	std::ifstream helpFile{ "resources/help.txt" };
	if (helpFile.is_open()) {
		std::ostringstream ss;
		ss << helpFile.rdbuf();
		m_HelpString = ss.str();
		helpFile.close();
	}
}

void VecMathListener::setCurrentCodeLine(std::string codeLine)
{
	m_CurrentCodeLine = codeLine;
}

void VecMathListener::exec(std::string code)
{
	using namespace VecMath;
	using namespace antlr4;
	try {
	ANTLRInputStream is{ code };
	VecMath::VecMathLexer lexer{ &is };

	CommonTokenStream stream{ &lexer };
	VecMathParser parser(&stream);
	parser.removeErrorListeners();
	parser.addParseListener(this);
	parser.addErrorListener(this);
	parser.expression();
	}
	catch (IllegalArgumentException ex) {
		printError("Illegal character in code, best to use ASCII characters only. ");
	}
}

IMatrix* VecMathListener::getVariable(const std::string& id) const
{
	if (m_VarMap.find(id) != m_VarMap.end()) {
		return m_VarMap.at(id);
	}
	else {
		return nullptr;
	}
}

void VecMathListener::exitCommand(VecMath::VecMathParser::CommandContext* ctx)
{
	if (ctx->EXIT() != nullptr) {
		printInfo("Leaving so soon? Was it something I said?");
		SetConsoleTextAttribute(m_ConsoleHandle, 15);
		m_Exit = true;

	}
	else if (ctx->PRINTALL() != nullptr) {
		for (const auto& [key, value] : m_VarMap) {
			SetConsoleTextAttribute(m_ConsoleHandle, 7);
			std::cout << key;
			SetConsoleTextAttribute(m_ConsoleHandle, 7);
			std::cout << " = ";
			value->print(m_ConsoleHandle);
			std::cout << "\n";
		}
	}
	else if (ctx->JOKE() != nullptr) {
		m_JokeGenerator.printRandomJoke(m_ConsoleHandle);
	}
	else if (ctx->HELP() != nullptr) {
		printMarkDown(m_HelpString);
	}
}

void VecMathListener::exitAssign(VecMath::VecMathParser::AssignContext* ctx)
{
	if (m_ErrorFlagged) {
		return;
	}
	auto idToken = ctx->ID();
	auto assignToken = ctx->ASSIGN();
	if (idToken != nullptr && idToken->getTreeType() != antlr4::tree::ParseTreeType::ERROR &&
		assignToken != nullptr && assignToken->getTreeType() != antlr4::tree::ParseTreeType::ERROR)
	{
		std::string varId = ctx->ID()->getText();
		if (stackIsValid()) {
			IMatrix* result = popFromStack();
			// check if the id is not a constant.
			if (m_Constants.find(varId) == m_Constants.end())
			{
				m_VarMap[varId] = result;
				if (ctx->SEMI() == nullptr) {
					printVariable(varId);
				}
			}
		}
		else {
			SetConsoleTextAttribute(m_ConsoleHandle, ERRORCOLOR);
			std::cout << "I do not know '" << varId << "'\n";
			SetConsoleTextAttribute(m_ConsoleHandle, INFOCOLOR);
			std::cout << "If you meant this as a command, this is a command I do not understand.\n";
			std::cout << "Type 'help' for a list of commands.\n";
			std::cout << "If you want this to be a variable you need the following format: b = 7\n";
		}
	}
	else if (ctx->value() != nullptr) {
		IMatrix* result = popFromStack();
		if (result != nullptr)
		{
			SetConsoleTextAttribute(m_ConsoleHandle, 8);
			result->print(m_ConsoleHandle);
			std::cout << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(m_ConsoleHandle, ERRORCOLOR);
		std::cout << "Assigning a new variable should be done with the equals sign.\n";
		std::cout << "For example : a = 10\n";
	}
}

void VecMathListener::exitPrint(VecMath::VecMathParser::PrintContext* ctx)
{
	if (ctx->ID() != nullptr && ctx->ID()->getTreeType() != antlr4::tree::ParseTreeType::ERROR)
	{
		std::string id = ctx->ID()->getText();
		printVariable(id);
	}
	else {
		SetConsoleTextAttribute(m_ConsoleHandle, ERRORCOLOR);
		std::cout << "Use the print statement with a variable id, for example: 'print var'\n";
	}
}

void VecMathListener::printVariable(const std::string& id) const
{
	if (m_VarMap.find(id) != m_VarMap.end()) {
		printVariable(id, m_VarMap.at(id));
	}
	else if (m_Constants.find(id) != m_Constants.end()) {
		printVariable(id, m_Constants.at(id));
	}
	else {
		SetConsoleTextAttribute(m_ConsoleHandle, 10);
		std::cout << "Could not find variable " << id << ",are you sure it exists? \n";
		std::cout << "Use the 'printAll' command to see the current list of variables.\n ";
	}
}

void VecMathListener::printVariable(const std::string& id, IMatrix* matrix) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, 7);
	std::cout << id;
	SetConsoleTextAttribute(m_ConsoleHandle, 7);
	std::cout << " = ";
	SetConsoleTextAttribute(m_ConsoleHandle, 8);
	matrix->print(m_ConsoleHandle);
	std::cout << std::endl;
}

void VecMathListener::exitClear(VecMath::VecMathParser::ClearContext* ctx)
{
	if (ctx->ID() != nullptr) {
		std::string id = ctx->ID()->getText();
		m_VarMap.erase(id);
	}
	else {
		m_VarMap.clear();
		clearScreen();
		promptHeader();
	}
}

void VecMathListener::exitLiteral(VecMath::VecMathParser::LiteralContext* ctx)
{
	if (ctx->FLOAT() != nullptr) {
		float x = std::stof(ctx->FLOAT()->getText());
		Scalar* s = new Scalar(x);
		m_ExprStack.push(s);
	}
}

void VecMathListener::exitVector(VecMath::VecMathParser::VectorContext* ctx)
{
	size_t dim = ctx->value().size();
	switch (dim) {
	case 1: {
		if (stackIsValid()) {
			IMatrix* f = popFromStack();
			Scalar* s = new Scalar(f->get(0, 0));
			m_ExprStack.push(s);
		}
		break;
	}
	case 2: {
		if (m_ExprStack.size() >= 2) {
			IMatrix* y = popFromStack();
			IMatrix* x = popFromStack();

			Vector2D* v2d = new Vector2D(x->get(0, 0), y->get(0, 0));
			m_ExprStack.push(v2d);
		}
		break;
	}
	case 3: {
		if (m_ExprStack.size() >= 3) {
			IMatrix* z = popFromStack();
			IMatrix* y = popFromStack();
			IMatrix* x = popFromStack();
			Vector3D* v3d = new Vector3D(x->get(0, 0), y->get(0, 0), z->get(0, 0));
			m_ExprStack.push(v3d);
		}
		break;
	}
	}
}

void VecMathListener::exitComplex(VecMath::VecMathParser::ComplexContext* ctx)
{
	if (m_ExprStack.size() >= 2) {
		IMatrix* i = popFromStack();
		IMatrix* r = popFromStack();
		Complex* q = new Complex(r->get(0, 0), i->get(0, 0));
		m_ExprStack.push(q);
	}
}

void VecMathListener::exitQuaternion(VecMath::VecMathParser::QuaternionContext* ctx)
{
	if (m_ExprStack.size() >= 4) {
		IMatrix* z = popFromStack();
		IMatrix* y = popFromStack();
		IMatrix* x = popFromStack();
		IMatrix* w = popFromStack();
		Quaternion* q = new Quaternion(x->get(0, 0), y->get(0, 0), z->get(0, 0), w->get(0, 0));
		m_ExprStack.push(q);
	}
}

void VecMathListener::exitValue(VecMath::VecMathParser::ValueContext* ctx)
{

	if (ctx->ID() != nullptr) {
		std::string id = ctx->ID()->getText();
		if (m_VarMap.find(id) != m_VarMap.end()) {
			m_ExprStack.push(m_VarMap[id]);
		}
		else if (m_Constants.find(id) != m_Constants.end()) {
			m_ExprStack.push(m_Constants[id]);
		}
		else {
			// yeah this is an error , what to do
			m_ExprStack.push(new Scalar(0));
		}
	}
	else if (ctx->op != nullptr) {

		using namespace VecMath;

		switch (ctx->op->getType()) {
		case VecMathLexer::ABS: {
			IMatrix* op2 = popFromStack();
			if (op2 != nullptr) {
				Scalar* s = new Scalar(op2->magnitude());
				m_ExprStack.push(s);
			}
			else {
				printError("An error occurred, maybe you forgot to asssign the value to a variable?");
			}

			break;
		}

		case VecMathLexer::ADD: {
			// unary case: do nothing leave value on stack.
			if (ctx->op2 != nullptr) {
				if (m_ExprStack.size() >= 2) {
					IMatrix* op2 = popFromStack();
					IMatrix* op1 = popFromStack();
					m_ExprStack.push(IMatrix::add(op1, op2));
				}
			}
			break;
		}
		case VecMathLexer::MINUS: {
			// can be unary minus!
			if (ctx->op1 == nullptr) {
				if (m_ExprStack.size() >= 1) {
					IMatrix* op = popFromStack();
					m_ExprStack.push(IMatrix::neg(op));
				}
			}
			else {
				if (m_ExprStack.size() >= 2) {
					IMatrix* op2 = popFromStack();
					IMatrix* op1 = popFromStack();
					m_ExprStack.push(IMatrix::subtract(op1, op2));
				}
			}
			break;
		}
		case VecMathLexer::MUL: {
			if (m_ExprStack.size() >= 2) {
				IMatrix* op2 = popFromStack();
				IMatrix* op1 = popFromStack();
				m_ExprStack.push(IMatrix::mul(op1, op2));
			}
			break;
		}
		case VecMathLexer::CROSS: {
			if (m_ExprStack.size() >= 2) {
				IMatrix* op2 = popFromStack();
				IMatrix* op1 = popFromStack();
				m_ExprStack.push(IMatrix::cross(op1, op2));
			}
			break;
		}
		case VecMathLexer::POWER: {
			if (m_ExprStack.size() >= 2) {
				IMatrix* op2 = popFromStack();
				IMatrix* op1 = popFromStack();
				m_ExprStack.push(IMatrix::power(op1, op2));
			}
			break;
		}
		case VecMathLexer::DIV: {
			if (m_ExprStack.size() >= 2) {
				IMatrix* op2 = popFromStack();
				IMatrix* op1 = popFromStack();
				m_ExprStack.push(IMatrix::divide(op1, op2));
			}
			break;
		}
		case VecMathLexer::DOT: {
			if (m_ExprStack.size() >= 2) {
				IMatrix* op2 = popFromStack();
				IMatrix* op1 = popFromStack();
				m_ExprStack.push(IMatrix::dot(op1, op2));
			}
			break;
		}
		}
	}
	else if (ctx->f != nullptr && !m_ExprStack.empty()) {
		IMatrix* op = popFromStack();
		IMatrix* result = nullptr;
		auto ctxFunc = ctx->f;
		if (ctxFunc->ID() == nullptr) {
			result = IMatrix::maxMatrix(op, nullptr);
			if (ctxFunc->COS() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return cos(x); }, result);
			}
			else if (ctxFunc->SIN() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return sin(x); }, result);
			}
			else if (ctxFunc->TAN() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return tan(x); }, result);
			}
			else if (ctxFunc->ACOS() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return acos(x); }, result);
			}
			else if (ctxFunc->ASIN() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return asin(x); }, result);
			}
			else if (ctxFunc->ATAN() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return atan(x); }, result);
			}
			else if (ctxFunc->DEGTORAD() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return x * m_DegToRad; }, result);
			}
			else if (ctxFunc->RADTODEG() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return x * m_RadToDeg; }, result);
			}
			else if (ctxFunc->SQRT() != nullptr) {
				IMatrix::unaryOp(op, [](float x) {return sqrt(x); }, result);
			}
			else if (ctxFunc->CON() != nullptr) {
				// only do this if it is a quaternion.
				result = op->conjugate();
			}
		}
		else {
			std::string funcName = ctxFunc->ID()->getText();
			if (funcName == "abs") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return abs(x); }, result);
			}
			else if (funcName == "norm") {
				result = new Scalar(op->magnitude());
			}
			else if (funcName == "inv") {
				result = op->inverse();
			}
			else if (funcName == "im") {
				result = op->imaginary();
			}
			else if (funcName == "re") {
				result = op->real();
			}
			else if (funcName == "axis") {
				result = op->axis();
			}
			else if (funcName == "angle") {
				result = op->angle();
			}
			else if (funcName == "angled") {
				result = op->angled();
			}
			else if (funcName == "sind") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return sin(x * m_DegToRad); }, result);
			}
			else if (funcName == "cosd") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return cos(x * m_DegToRad); }, result);
			}
			else if (funcName == "tand") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return tan(x * m_DegToRad); }, result);
			}
			else if (funcName == "asind") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return m_RadToDeg * asin(x); }, result);
			}
			else if (funcName == "acosd") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return m_RadToDeg * acos(x); }, result);
			}
			else if (funcName == "atand") {
				result = IMatrix::maxMatrix(op, nullptr);
				IMatrix::unaryOp(op, [](float x) {return m_RadToDeg * atan(x); }, result);
			}
			else {
				printError("There is no function called " + funcName + " (yet)!");
				printInfo("Maybe you made a typo? (type 'help' for a list of functions)");
				m_ErrorFlagged = true;
			}
		}
		if (result != nullptr && !m_ErrorFlagged) {
			pushToStack(result);
		}
	}
}

void VecMathListener::exitFunction(VecMath::VecMathParser::FunctionContext* ctx)
{

}

void VecMathListener::exitPrecision(VecMath::VecMathParser::PrecisionContext* ctx)
{
	if (ctx->FLOAT() != nullptr) {
		float p = std::stof(ctx->FLOAT()->getText());
		int precision = static_cast<int>(round(p));
		if (precision >= 0) {
			std::cout << std::fixed << std::showpoint << std::setprecision(precision);
		}
		else {
			printInfo("The precision must be a positive number, specifying the number of decimal places.");
			printInfo("For example: precision 4");
		}
	}
	else {
		printInfo("Specify the number of decimal places for this shell.");
		printInfo("For example : precision 3");
	}
}

void VecMathListener::visitErrorNode(antlr4::tree::ErrorNode* node) {
	if (m_ErrorFlagged) {
		// no need to report multiple errors.
		return;
	}
	size_t type = node->getSymbol()->getType();

	auto interval = node->getSourceInterval();


	switch (type) {
	case VecMath::VecMathLexer::COMMA:
		printErrorLoc(interval.a, interval.b, m_CurrentCodeLine);
		printError("Was not expecting a comma at character " + std::to_string(interval.a + 1));
		printInfo("Did you forget to use [ and ] to define vectors, matrices or quaternions?");
		printInfo("Example: v=[2,3.1,7]");
		m_ErrorFlagged = true;
		break;
	case VecMath::VecMathLexer::FLOAT:
		printErrorLoc(interval.a, interval.b, m_CurrentCodeLine);
		printError("Was not expecting a float at character " + std::to_string(interval.a + 1));
		printInfo("Did you forget to use commas to define vectors, matrices or quaternions?");
		printInfo("Example: v=[2,3.1,7]");
		m_ErrorFlagged = true;
		break;
	}

}

void VecMathListener::printErrorLoc(size_t start, size_t end, const std::string& message)
{
	SetConsoleTextAttribute(m_ConsoleHandle, INFOCOLOR);
	for (size_t pc = 0; pc < message.size(); ++pc) {
		if (pc < start || pc > end) {
			SetConsoleTextAttribute(m_ConsoleHandle, TEXTCOLOR);
		}
		else {
			SetConsoleTextAttribute(m_ConsoleHandle, ERRORCOLOR);
		}
		std::cout << message[pc];
	}
	std::cout << "\n";
}

void VecMathListener::printMarkDown(const std::string& text)
{
	std::stringstream ss(text);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		if (token[0] == '#') {
			printInfo(token.substr(1));
		}
		else {
			printText(token);
		}
	}
}

void VecMathListener::clearScreen()
{
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
}

void VecMathListener::clearVariables()
{
	for (auto pair : m_VarMap) {
		delete pair.second;
	}
	m_VarMap.clear();
	for (auto pair : m_Constants) {
		delete pair.second;
	}
	m_Constants.clear();
}


void VecMathListener::printInfo(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, INFOCOLOR);
	std::cout << message << "\n";
}

void VecMathListener::printError(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, ERRORCOLOR);
	std::cout << message << "\n";
}

void VecMathListener::printText(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, TEXTCOLOR);
	std::cout << message << "\n";
}

void VecMathListener::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e)
{
	// basic check of matched brackets and parenthesis.
	size_t lp = std::count(m_CurrentCodeLine.begin(), m_CurrentCodeLine.end(), '(');
	size_t rp = std::count(m_CurrentCodeLine.begin(), m_CurrentCodeLine.end(), ')');
	if (lp != rp)
	{
		printErrorLoc(charPositionInLine, charPositionInLine, m_CurrentCodeLine);
		if (rp < lp) {
			printInfo("Missing ')', this became apparent at location " + std::to_string(charPositionInLine));
		}
		else {
			printInfo("Missing '(', this became apparent at location " + std::to_string(charPositionInLine));
		}

		m_ErrorFlagged = true;
	}
	size_t lb = std::count(m_CurrentCodeLine.begin(), m_CurrentCodeLine.end(), '[');
	size_t rb = std::count(m_CurrentCodeLine.begin(), m_CurrentCodeLine.end(), ']');
	if (lb != rb)
	{
		printErrorLoc(charPositionInLine, charPositionInLine, m_CurrentCodeLine);
		if (rb < lb) {
			printInfo("Missing ']', this became apparent at location " + std::to_string(charPositionInLine));
		}
		else {
			printInfo("Missing '[', this became apparent at location " + std::to_string(charPositionInLine));
		}
		m_ErrorFlagged = true;
	}
}

void VecMathListener::reportAmbiguity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, bool exact, const antlrcpp::BitSet& ambigAlts, antlr4::atn::ATNConfigSet* configs)
{
}

void VecMathListener::reportAttemptingFullContext(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, const antlrcpp::BitSet& conflictingAlts, antlr4::atn::ATNConfigSet* configs)
{
}

void VecMathListener::reportContextSensitivity(antlr4::Parser* recognizer, const antlr4::dfa::DFA& dfa, size_t startIndex, size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet* configs)
{
}

IMatrix* VecMathListener::popFromStack()
{
	if (m_ExprStack.size() > 0) {
		IMatrix* r = m_ExprStack.top();
		m_ExprStack.pop();
		return r;
	}
	else {
		return nullptr;
	}
}

void VecMathListener::pushToStack(IMatrix* toPush)
{
	m_ExprStack.push(toPush);
}
