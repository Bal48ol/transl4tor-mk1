#pragma once
#include <memory>
#include "Atoms.h"
#include <vector>
#include <string>
#include <ostream>

class StringTable
{
protected:
	std::vector<std::string> _strings;

public:
	const std::string& operator [](const int index) const;
	std::shared_ptr<StringOperand> add(const std::string& name);
	friend std::ostream& operator<<(std::ostream& stream, const StringTable& st);
};