#include "SymbolTable.h"
#include "Atoms.h"

bool SymbolTable::TableRecord::operator==(const TableRecord& other) const
{
	return other._name == _name;
}

const SymbolTable::TableRecord& SymbolTable::operator[](const int index) const
{
	if (index >= 0 && index < _records.size()) {
		return _records[index];
	}
	throw std::runtime_error("Index is out of range");
}

std::shared_ptr<MemoryOperand> SymbolTable::add(const std::string& name)
{
	auto find_result = std::find_if(_records.begin(), _records.end(), [name](const TableRecord& t) { return t._name == name; });
	if (find_result == _records.end()) {
		_records.push_back({ name });
		return std::make_shared<MemoryOperand>(MemoryOperand(_records.size() - 1, this));
	}
	return std::make_shared<MemoryOperand>(MemoryOperand(_records.size() - (_records.end() - find_result), this));
}

std::shared_ptr<MemoryOperand> SymbolTable::alloc()
{
	_records.push_back({ "" });
	return std::make_shared<MemoryOperand>(_records.size() - 1, this);
}

std::ostream& operator<<(std::ostream& stream, const SymbolTable& st)
{
	stream << "Symbol Table\n";
	int line = 0;
	for (auto& item : st._records) {
		stream << std::setiosflags(std::ios::left) << std::setw(3) << line++;
		if (item._name.empty()) {
			stream << "TMP" + std::to_string(line - 1) << '\n';
		}
		else {
			stream << item._name << '\n';
		}
	}
	return stream;
}