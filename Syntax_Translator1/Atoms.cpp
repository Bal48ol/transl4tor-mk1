#include "Atoms.h"
#include "StringTable.h"
#include "SymbolTable.h"

MemoryOperand::MemoryOperand(int index, const SymbolTable* symbolTable) : _index(index), _symbolTable(symbolTable)
{
}

std::string MemoryOperand::toString() const
{
	return std::to_string(_index);
}

bool MemoryOperand::operator==(const MemoryOperand& mo)
{
	return _index == mo._index && _symbolTable == mo._symbolTable;
}

std::string LabelOperand::toString() const
{
	return std::to_string(_labelID);
}

NumberOperand::NumberOperand(int value) : _value(value)
{
}

std::string NumberOperand::toString() const
{
	return "\"" + std::to_string(_value) + "\"";
}

StringOperand::StringOperand(int index, const StringTable* stringTable) : _index(index), _stringTable(stringTable)
{
}

std::string StringOperand::toString() const
{
	return std::to_string(_index);
}

bool StringOperand::operator==(const StringOperand& so)
{
	return _index == so._index && _stringTable == so._stringTable;
}

LabelOperand::LabelOperand(int labelID) : _labelID(labelID)
{
}

RValue::RValue()
{
}

Atom::Atom()
{
}

UnaryOpAtom::UnaryOpAtom(const std::string& name, std::shared_ptr<RValue> operand, std::shared_ptr<MemoryOperand> result) : _name(name), _operand(operand), _result(result)
{
}

std::string UnaryOpAtom::toString() const
{
	return "(" + _name + ", " + _operand->toString() + ",, " + _result->toString() + ")";
}

BinaryOpAtom::BinaryOpAtom(const std::string& name, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<MemoryOperand> result) : _name(name), _left(left), _right(right), _result(result)
{
}

std::string BinaryOpAtom::toString() const
{
	return "(" + _name + ", " + _left->toString() + ", " + _right->toString() + ", " + _result->toString() + ")";
}

OutAtom::OutAtom(std::shared_ptr<Operand> value) : _value(value)
{
}

std::string OutAtom::toString() const
{
	return "(OUT, " + _value->toString() + ")";
}

LabelAtom::LabelAtom(std::shared_ptr<LabelOperand> label) : _label(label)
{
}

std::string LabelAtom::toString() const
{
	return "(LBL,,, " + _label->toString() + ")";
}

JumpAtom::JumpAtom(std::shared_ptr<LabelOperand> label) : _label(label)
{
}

std::string JumpAtom::toString() const
{
	return "(JMP,,, " + _label->toString() + ")";
}

ConditionalJumpAtom::ConditionalJumpAtom(const std::string& condition, std::shared_ptr<RValue> left, std::shared_ptr<RValue> right, std::shared_ptr<LabelOperand> label)
	: _condition(condition), _left(left), _right(right), _label(label)
{
}

std::string ConditionalJumpAtom::toString() const
{
	return "(" + _condition + ", " + _left->toString() + ", " + _right->toString() + ", " + _label->toString() + ")";
}

InAtom::InAtom(std::shared_ptr<MemoryOperand> result) : _result(result)
{
}

std::string InAtom::toString() const
{
	return "(IN, " + _result->toString() + ")";
}