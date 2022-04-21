#pragma once
#include <vector>
#include <memory>
#include "Atoms.h"
#include "StringTable.h"
#include "SymbolTable.h"
#include "Scanner.h"

class translator
{
public:
	translator(std::istream& stream);
	void printAtoms(std::ostream& stream);
	void generateAtom(std::unique_ptr<Atom> atom);

	std::shared_ptr<LabelOperand> newLabel();
	void syntaxError(const std::string& message);
	void lexicalError(const std::string& message);

protected:
	std::vector<std::unique_ptr<Atom>> _atoms;
	StringTable _stringTable;
public:
	SymbolTable _symbolTable;

	Scanner _scanner;
	Token _currentLexem;
	int _currentLabel;

	inline void lexCheck();

private: 
	std::shared_ptr<RValue> E();
	std::shared_ptr<RValue> E7();
	std::shared_ptr<RValue> E7_(std::shared_ptr<RValue>);
	std::shared_ptr<RValue> E6();
	std::shared_ptr<RValue> E6_(std::shared_ptr<RValue>);
	std::shared_ptr<RValue> E5();
	std::shared_ptr<RValue> E5_(std::shared_ptr<RValue>);
	std::shared_ptr<RValue> E4();
	std::shared_ptr<RValue> E4_(std::shared_ptr<RValue>);
	std::shared_ptr<RValue> E3();
	std::shared_ptr<RValue> E3_(std::shared_ptr<RValue>);
	std::shared_ptr<RValue> E2();
	std::shared_ptr<RValue> E1();
	std::shared_ptr<RValue> E1_(std::shared_ptr<RValue>);
};
