#include "translator.h"

translator::translator(std::istream& stream) : _scanner(stream), _currentLexem(_scanner.getNextToken())
{
	E();
}

void translator::printAtoms(std::ostream& stream)
{
	for (auto& atom : _atoms) {
		stream << atom->toString() << std::endl;
	}
}

void translator::generateAtom(std::unique_ptr<Atom> atom)
{
	_atoms.push_back(std::move(atom));
}

std::shared_ptr<LabelOperand> translator::newLabel()
{
	return std::make_shared<LabelOperand>(_currentLabel);
}

void translator::syntaxError(const std::string& message)
{
	throw "Syntax error: " + message;
}

void translator::lexicalError(const std::string& message)
{
	throw "Lexical error: " + message;
}

void translator::lexCheck()
{
	if (_currentLexem.type() == LexemType::error) {
		lexicalError(_currentLexem.str());
	}
}

std::shared_ptr<RValue> translator::E() {
	return E7();
}

std::shared_ptr<RValue> translator::E7() {
	auto q = E6();
	return E7_(q);
}

std::shared_ptr<RValue> translator::E7_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentLexem.type() == LexemType::opor) {
		_currentLexem = _scanner.getNextToken();
		auto r = E6();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("OR", p, r, s));
		return E7_(s);
	}
	return p;
}

std::shared_ptr<RValue> translator::E6() {
	auto q = E5();
	return E6_(q);
}

std::shared_ptr<RValue> translator::E6_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentLexem.type() == LexemType::opand) {
		_currentLexem = _scanner.getNextToken();
		auto r = E5();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("AND", p, r, s));
		return E6_(s);
	}
	return p;
}

std::shared_ptr<RValue> translator::E4() {
	auto q = E3();
	return E4_(q);
}

std::shared_ptr<RValue> translator::E4_(std::shared_ptr<RValue> p) {
	lexCheck();
	LexemType type = _currentLexem.type();
	if (type == LexemType::opplus || type == LexemType::opminus) {
		std::map<LexemType, std::string> types{ {LexemType::opplus, "ADD"}, {LexemType::opminus, "SUB"} };
		_currentLexem = _scanner.getNextToken();
		auto r = E3();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>(types[type], p, r, s));
		return E4_(s);
	}
	return p;
}

std::shared_ptr<RValue> translator::E1() {
	lexCheck();
	if (_currentLexem.type() == LexemType::num) {
		auto q = std::make_shared<NumberOperand>(_currentLexem.value());
		_currentLexem = _scanner.getNextToken();
		return q; // 25
	}
	if (_currentLexem.type() == LexemType::lpar) {
		_currentLexem = _scanner.getNextToken();
		auto q = E();
		lexCheck();
		if (_currentLexem.type() != LexemType::rpar) syntaxError("Not enought ')'");
		_currentLexem = _scanner.getNextToken();
		return q; // 24
	}
	if (_currentLexem.type() == LexemType::chr) {
		auto q = std::make_shared<NumberOperand>(_currentLexem.value());
		_currentLexem = _scanner.getNextToken();
		return q; // 26
	}
	if (_currentLexem.type() == LexemType::opinc) {
		_currentLexem = _scanner.getNextToken();
		lexCheck();
		auto p = _symbolTable.add(_currentLexem.str());
		auto one = std::make_shared<NumberOperand>(1);
		generateAtom(std::make_unique<BinaryOpAtom>("ADD", p, one, std::dynamic_pointer_cast<MemoryOperand>(p)));
		_currentLexem = _scanner.getNextToken();
		lexCheck();
		return p; // 29
	}
	if (_currentLexem.type() == LexemType::id) {
		auto r = _symbolTable.add(_currentLexem.str());
		_currentLexem = _scanner.getNextToken();
		return E1_(r);
	}
}

std::shared_ptr<RValue> translator::E1_(std::shared_ptr<RValue> p) { // e ++
	lexCheck();
	if (_currentLexem.type() == LexemType::opinc) {
		_currentLexem = _scanner.getNextToken();
		auto r = _symbolTable.alloc();
		auto one = std::make_shared<NumberOperand>(1);
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", p, r));
		generateAtom(std::make_unique<BinaryOpAtom>("ADD", p, one, std::dynamic_pointer_cast<MemoryOperand>(p)));
		return r; // 29
	}
	return p; // 31
}

std::shared_ptr<RValue> translator::E3() {
	auto q = E2();
	return E3_(q); // 19
}

std::shared_ptr<RValue> translator::E3_(std::shared_ptr<RValue> p) {
	lexCheck();
	if (_currentLexem.type() == LexemType::opmult) {
		_currentLexem = _scanner.getNextToken();
		auto r = E2();
		auto s = _symbolTable.alloc();
		generateAtom(std::make_unique<BinaryOpAtom>("MUL", p, r, s));
		return E3_(s);
	} // 20
	return p; // 21
}

std::shared_ptr<RValue> translator::E5() {
	auto q = E4();
	return E5_(q);
}

std::shared_ptr<RValue> translator::E5_(std::shared_ptr<RValue> p) {
	lexCheck();
	LexemType type = _currentLexem.type();
	if (type == LexemType::opeq || type == LexemType::ople || type == LexemType::opgt || type == LexemType::oplt || type == LexemType::opne) {
		std::map<LexemType, std::string> jumps = { {LexemType::opeq, "EQ"}, {LexemType::opne, "NE"}, {LexemType::opgt, "GT"},
			{LexemType::oplt, "LT"}, {LexemType::opne, "NE"} };
		_currentLexem = _scanner.getNextToken();
		lexCheck();
		auto r = E4();
		auto s = _symbolTable.alloc();
		auto l = newLabel();
		auto one = std::make_shared<NumberOperand>(1);
		auto zero = std::make_shared<NumberOperand>(0);
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", one, s));
		generateAtom(std::make_unique<ConditionalJumpAtom>(jumps[type], p, r, l));
		generateAtom(std::make_unique<UnaryOpAtom>("MOV", zero, s));
		generateAtom(std::make_unique<LabelAtom>(l));
		return s; // 13
	}
	return p; // 14
}

std::shared_ptr<RValue> translator::E2() {
	lexCheck();
	if (_currentLexem.type() == LexemType::opnot) {
		auto q = E1();
		auto r = _symbolTable.alloc();
		generateAtom(std::make_unique<UnaryOpAtom>("NOT", q, std::dynamic_pointer_cast<MemoryOperand>(r)));
		_currentLexem = _scanner.getNextToken();
		return r;
	}
	return E1();
}