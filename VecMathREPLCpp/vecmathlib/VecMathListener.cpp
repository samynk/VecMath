#include "VecMathListener.h"
#include "Scalar.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Complex.h"
#include "Quaternion.h"
#include "VecMathLexer.h"


#undef ERROR

VecMathListener::VecMathListener()
{
	float fPI = static_cast<float>(M_PI);
	addToConstantMap("Pi", new Scalar(fPI));
	addToConstantMap("PI", new Scalar(fPI));
	addToConstantMap("pi", new Scalar(fPI));
	addToConstantMap("e", new Scalar(static_cast<float>(M_E)));
	readHelp();
}

void VecMathListener::prompt(const std::string& text)
{
	m_Console.Print(Console::VMF_CYAN, text);
	m_ErrorFlagged = false;
	if (!m_ExprStack.empty())
	{
		m_Console.Print(Console::VMF_GRAY, "Expression stack not empty :");
		m_Console.Print(Console::VMF_GRAY, std::to_string(m_ExprStack.size()));
	}
	while (!m_ExprStack.empty()) {
		m_ExprStack.pop();
	}
}

void VecMathListener::promptHeader() const
{
	printInfo("VecMath REPL shell 1.0.7 - Koen Samyn - 2022");
	printInfo("please report issues at https://github.com/samynk/VecMath");
}

void VecMathListener::readHelp()
{
#ifdef __EMSCRIPTEN__
	std::ifstream helpFile{ "/resources/help.txt" };
#else
	std::ifstream helpFile{ "resources/help.txt" };
#endif
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
	catch(InputMismatchException ex)
	{
		printError("Input mismatch exception");
	}
}

std::shared_ptr<IMatrix> VecMathListener::getVariable(const std::string& id) const
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
		m_Console.Reset();
		m_Exit = true;

	}
	else if (ctx->PRINTALL() != nullptr) {
		for (const auto& [key, value] : m_VarMap) {
			m_Console.Print(Console::VMF_LIGHTGRAY, key);
			m_Console.Print(Console::VMF_LIGHTGRAY, " = ");
			value->print(m_Console);
			m_Console.NewLine();
		}
	}
	else if (ctx->JOKE() != nullptr) {
		m_JokeGenerator.printRandomJoke(m_Console);
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
			auto result = popFromStack();
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
			m_Console.SetColor(Console::VMF_BRIGHTRED);
			std::cout << "I do not know '" << varId << "'\n";
			m_Console.SetColor(Console::VMF_BRIGHTGREEN);
			std::cout << "If you meant this as a command, this is a command I do not understand.\n";
			std::cout << "Type 'help' for a list of commands.\n";
			std::cout << "If you want this to be a variable you need the following format: b = 7\n";
		}
	}
	else if (ctx->value() != nullptr && stackIsValid()) {
		auto result = popFromStack();
		if (result != nullptr)
		{
			result->print(m_Console);
			std::cout << std::endl;
		}
	}
	else
	{
		m_Console.SetColor(Console::VMF_BRIGHTRED);
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
		m_Console.SetColor(Console::VMF_BRIGHTRED);
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
		m_Console.SetColor(Console::VMF_BRIGHTGREEN);
		std::cout << "Could not find variable " << id << ",are you sure it exists? \n";
		std::cout << "Use the 'printAll' command to see the current list of variables.\n ";
	}
}

void VecMathListener::printVariable(const std::string& id, std::shared_ptr<IMatrix> matrix) const
{
	m_Console.Print(Console::VMF_LIGHTGRAY, id);
	m_Console.Print(Console::VMF_LIGHTGRAY, " = ");
	matrix->print(m_Console);
	m_Console.NewLine();
}

void VecMathListener::exitClear(VecMath::VecMathParser::ClearContext* ctx)
{
	if (ctx->ID() != nullptr) {
		std::string id = ctx->ID()->getText();
		m_VarMap.erase(id);
	}
	else {
		m_VarMap.clear();
		m_Console.ClearScreen();
		promptHeader();
	}
}

void VecMathListener::exitLiteral(VecMath::VecMathParser::LiteralContext* ctx)
{
	if (ctx->FLOAT() != nullptr) {
		float x = std::stof(ctx->FLOAT()->getText());
		Scalar* s = new Scalar(x);
		pushToExprStack(s);
	}
}

void VecMathListener::exitVector(VecMath::VecMathParser::VectorContext* ctx)
{
	size_t dim = ctx->value().size();
	switch (dim) {
	case 1: {
		if (stackIsValid()) {
			auto f = popFromStack();
			Scalar* s = new Scalar(f->get(0, 0));
			pushToExprStack(s);
		}
		break;
	}
	case 2: {
		if (m_ExprStack.size() >= 2) {
			auto y = popFromStack();
			auto x = popFromStack();

			Vector2D* v2d = new Vector2D(x->get(0, 0), y->get(0, 0));
			pushToExprStack(v2d);
		}
		break;
	}
	case 3: {
		if (m_ExprStack.size() >= 3) {
			auto z = popFromStack();
			auto y = popFromStack();
			auto x = popFromStack();
			Vector3D* v3d = new Vector3D(x->get(0, 0), y->get(0, 0), z->get(0, 0));
			pushToExprStack(v3d);
		}
		break;
	}
	}
}

void VecMathListener::exitComplex(VecMath::VecMathParser::ComplexContext* ctx)
{
	if (m_ExprStack.size() >= 2) {
		auto i = popFromStack();
		auto r = popFromStack();
		Complex* q = new Complex(r->get(0, 0), i->get(0, 0));
		pushToExprStack(q);
	}
}

void VecMathListener::exitQuaternion(VecMath::VecMathParser::QuaternionContext* ctx)
{
	if (m_ExprStack.size() >= 4) {
		auto z = popFromStack();
		auto y = popFromStack();
		auto x = popFromStack();
		auto w = popFromStack();
		Quaternion* q = new Quaternion(x->get(0, 0), y->get(0, 0), z->get(0, 0), w->get(0, 0));
		pushToExprStack(q);
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
			pushToExprStack(new Scalar(0));
		}
	}
	else if (ctx->op != nullptr) {

		using namespace VecMath;

		switch (ctx->op->getType()) {
		case VecMathLexer::ABS: {
			auto op2 = popFromStack();
			if (op2 != nullptr) {
				Scalar* s = new Scalar(op2->magnitude());
				pushToExprStack(s);
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
					auto op2 = popFromStack();
					auto op1 = popFromStack();
					pushToExprStack(IMatrix::add(op1.get(), op2.get()));
				}
			}
			break;
		}
		case VecMathLexer::MINUS: {
			// can be unary minus!
			if (ctx->op1 == nullptr) {
				if (m_ExprStack.size() >= 1) {
					auto op = popFromStack();
					pushToExprStack(IMatrix::neg(op.get()));
				}
			}
			else {
				if (m_ExprStack.size() >= 2) {
					auto op2 = popFromStack();
					auto op1 = popFromStack();
					pushToExprStack(IMatrix::subtract(op1.get(), op2.get()));
				}
			}
			break;
		}
		case VecMathLexer::MUL: {
			if (m_ExprStack.size() >= 2) {
				auto op2 = popFromStack();
				auto op1 = popFromStack();
				pushToExprStack(IMatrix::mul(op1.get(), op2.get()));
			}
			break;
		}
		case VecMathLexer::CROSS: {
			if (m_ExprStack.size() >= 2) {
				auto op2 = popFromStack();
				auto op1 = popFromStack();
				pushToExprStack(IMatrix::cross(op1.get(), op2.get()));
			}
			break;
		}
		case VecMathLexer::POWER: {
			if (m_ExprStack.size() >= 2) {
				auto op2 = popFromStack();
				auto op1 = popFromStack();
				pushToExprStack(IMatrix::power(op1.get(), op2.get()));
			}
			break;
		}
		case VecMathLexer::DIV: {
			if (m_ExprStack.size() >= 2) {
				auto op2 = popFromStack();
				auto op1 = popFromStack();
				pushToExprStack(IMatrix::divide(op1.get(), op2.get()));
			}
			break;
		}
		case VecMathLexer::DOT: {
			if (m_ExprStack.size() >= 2) {
				auto op2 = popFromStack();
				auto op1 = popFromStack();
				pushToExprStack(IMatrix::dot(op1.get(), op2.get()));
			}
			break;
		}
		}
	}
	else if (ctx->f != nullptr && !m_ExprStack.empty()) {
		auto op = popFromStack();
		IMatrix* result = nullptr;
		auto ctxFunc = ctx->f;
		IMatrix* rawOp = op.get();
		if (ctxFunc->ID() == nullptr) {
			result = IMatrix::maxMatrix(rawOp, nullptr);
			if (ctxFunc->COS() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return cos(x); }, result);
			}
			else if (ctxFunc->SIN() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return sin(x); }, result);
			}
			else if (ctxFunc->TAN() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return tan(x); }, result);
			}
			else if (ctxFunc->ACOS() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return acos(x); }, result);
			}
			else if (ctxFunc->ASIN() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return asin(x); }, result);
			}
			else if (ctxFunc->ATAN() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return atan(x); }, result);
			}
			else if (ctxFunc->DEGTORAD() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return x * m_DegToRad; }, result);
			}
			else if (ctxFunc->RADTODEG() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return x * m_RadToDeg; }, result);
			}
			else if (ctxFunc->SQRT() != nullptr) {
				IMatrix::unaryOp(rawOp, [](float x) {return sqrt(x); }, result);
			}
			else if (ctxFunc->CON() != nullptr) {
				// only do this if it is a quaternion.
				result = op->conjugate();
			}
		}
		else {
			std::string funcName = ctxFunc->ID()->getText();
			if (funcName == "abs") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return abs(x); }, result);
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
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return sin(x * m_DegToRad); }, result);
			}
			else if (funcName == "cosd") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return cos(x * m_DegToRad); }, result);
			}
			else if (funcName == "tand") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return tan(x * m_DegToRad); }, result);
			}
			else if (funcName == "asind") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return m_RadToDeg * asin(x); }, result);
			}
			else if (funcName == "acosd") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return m_RadToDeg * acos(x); }, result);
			}
			else if (funcName == "atand") {
				result = IMatrix::maxMatrix(rawOp, nullptr);
				IMatrix::unaryOp(rawOp, [](float x) {return m_RadToDeg * atan(x); }, result);
			}
			else {
				printError("There is no function called " + funcName + " (yet)!");
				printInfo("Maybe you made a typo? (type 'help' for a list of functions)");
				m_ErrorFlagged = true;
			}
		}
		if (result != nullptr && !m_ErrorFlagged) {
			pushToExprStack(result);
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
			m_Console.SetPrecision(precision);
			
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
	for (size_t pc = 0; pc < message.size(); ++pc) {
		if (pc < start || pc > end) {
			m_Console.SetColor(Console::VMF_LIGHTGRAY);
		}
		else {
			m_Console.SetColor(Console::VMF_BRIGHTRED);
		}
		m_Console.Print(message[pc]);
	}
	m_Console.NewLine();
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

void VecMathListener::clearVariables()
{
	/*for (auto pair : m_VarMap) {
		delete pair.second;
	}*/
	m_VarMap.clear();
	/*for (auto pair : m_Constants) {
		delete pair.second;
	}*/
	m_Constants.clear();
}


void VecMathListener::printInfo(const std::string& message) const
{
	m_Console.Print(Console::VMF_BRIGHTGREEN, message);
	m_Console.NewLine();
}

void VecMathListener::printError(const std::string& message) const
{
	m_Console.Print(Console::VMF_BRIGHTRED, message);
	m_Console.NewLine();
}

void VecMathListener::printText(const std::string& message) const
{
	m_Console.Print(Console::VMF_LIGHTGRAY, message);
	m_Console.NewLine();
}

void VecMathListener::printDebug(const std::string& message) const
{
#ifndef NDEBUG
	m_Console.Print(Console::VMF_LIGHTGRAY, message);
	m_Console.NewLine();
#endif
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

std::shared_ptr<IMatrix> VecMathListener::popFromStack()
{
	if (!m_ExprStack.empty()) {
		auto r = m_ExprStack.top();
		m_ExprStack.pop();
		return r;
	}
	else {
		return nullptr;
	}
}

void VecMathListener::pushToExprStack(IMatrix* toPush)
{
	m_ExprStack.push(std::shared_ptr<IMatrix>(toPush));
}

void VecMathListener::addToConstantMap(std::string constantName, IMatrix* constant)
{
	m_Constants[constantName] = std::shared_ptr<IMatrix>(constant);
}

void VecMathListener::setUseColors(bool colors)
{
	m_Console.SetUseColors(colors);
}


