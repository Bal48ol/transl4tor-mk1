#pragma once
#include <string>
#include <memory>

class SymbolTable;
class StringTable;

class Operand {
public:
	virtual std::string toString() const = 0;
};

class RValue : public Operand {
public:
	RValue();
};

class MemoryOperand : public RValue {
public:
	MemoryOperand(int index, const SymbolTable* symbolTable);
	std::string toString() const override;

	bool operator==(const MemoryOperand&);

protected:
	int _index;
	const SymbolTable* _symbolTable;
};

class NumberOperand : public RValue {
public:
	NumberOperand(int value);
	std::string toString() const override;

protected:
	int _value;
};

class StringOperand : public Operand {
public:
	StringOperand(int index, const StringTable* stringTable);
	std::string toString() const override;

	bool operator==(const StringOperand&);

protected:
	int _index;
	const StringTable* _stringTable;
};

class LabelOperand : public Operand {
public:
	LabelOperand(int labelID);
	std::string toString() const override;

protected:
	int _labelID;
};

class Atom {
public:
	Atom();
	virtual std::string toString() const = 0;
};

class UnaryOpAtom : public Atom {
public:
	UnaryOpAtom(const std::string& name, std::shared_ptr<RValue> operand, std::shared_ptr<MemoryOperand> result);
	std::string toString() const override;

protected:
	std::string _name;
	std::shared_ptr<RValue> _operand;
	std::shared_ptr<MemoryOperand> _result;
};

class BinaryOpAtom : public Atom {
public:
	BinaryOpAtom(const std::string& name, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<MemoryOperand> result);
	std::string toString() const override;

protected:
	std::string _name;
	std::shared_ptr<RValue> _left;
	std::shared_ptr<RValue> _right;
	std::shared_ptr<MemoryOperand> _result;
};

class OutAtom : public Atom {
public:
	OutAtom(std::shared_ptr<Operand> value);
	std::string toString() const override;

protected:
	std::shared_ptr<Operand> _value;
};

class InAtom : public Atom {
public:
	InAtom(std::shared_ptr<MemoryOperand> result);
	std::string toString() const override;

protected:
	std::shared_ptr<MemoryOperand> _result;
};

class LabelAtom : public Atom {
public:
	LabelAtom(std::shared_ptr<LabelOperand> label);
	std::string toString() const override;

protected:
	std::shared_ptr<LabelOperand> _label;
};

class JumpAtom : public Atom {
public:
	JumpAtom(std::shared_ptr<LabelOperand> label);
	std::string toString() const override;

protected:
	std::shared_ptr<LabelOperand> _label;
};

class ConditionalJumpAtom : public Atom {
public:
	ConditionalJumpAtom(const std::string& condition, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<LabelOperand> label);
	std::string toString() const override;

protected:
	std::string _condition;
	std::shared_ptr<RValue> _left;
	std::shared_ptr<RValue> _right;
	std::shared_ptr<LabelOperand> _label;
};