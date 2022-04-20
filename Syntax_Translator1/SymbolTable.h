#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "Atoms.h"
#include <iomanip>

class SymbolTable
{
public:
	struct TableRecord {
		std::string _name;
		bool operator==(const TableRecord& other) const;
	};

	const TableRecord& operator[](const int index) const;
	std::shared_ptr<MemoryOperand> add(const std::string& name);
	std::shared_ptr<MemoryOperand> alloc();
	friend std::ostream& operator<<(std::ostream& stream, const SymbolTable&);

protected:
	std::vector<TableRecord> _records;
};