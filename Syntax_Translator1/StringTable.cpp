#include "StringTable.h"
#include <algorithm>
#include <stdexcept>

const std::string& StringTable::operator[](const int index) const
{
	if (index >= 0 && index < _strings.size()) {
		return _strings[index];
	}
	throw std::runtime_error("Index is out of range");
}

std::shared_ptr<StringOperand> StringTable::add(const std::string& name)
{
	auto find_result = std::find(_strings.begin(), _strings.end(), name);
	if (find_result == _strings.end()) {
		_strings.push_back(name);
		return std::make_shared<StringOperand>(StringOperand(_strings.size() - 1, this));
	}
	return std::make_shared<StringOperand>(StringOperand(_strings.end() - find_result - 1, this));
}

std::ostream& operator<<(std::ostream& stream, const StringTable& st)
{
	for (const std::string& string : st._strings) {
		stream << string << ' ';
	}
	return stream;
}
