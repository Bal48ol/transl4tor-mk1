#include <iostream>
#include "translator.h"
#include <string>
#include <vector>
#include <sstream>

int main()
{
	std::stringstream str("b*b+4*a*c");
	translator tr(str);
	tr.printAtoms(std::cout);
}