#include "parser.h"
#include "ParserAPI.hpp"
#include <iostream>
#include <map>

//using namespace Parser;

int main() {
	
	Parser<ui32> a;
	a["msg"] = (i64)9;
	a["helo"]["idk"] = 3.6;
	a["helo"]["haha"] = "amogus";
	a["helo"].setName("hello");
	a["hello"][0].getAs<double>() -= 5;

	__bytes* data = a.pack();

	Parser<ui32> b;
	b.parse(data);

	std::cout << b["hello"]["idk"].getAs<double>() << "   " << b["hello"]["haha"].getAs<std::string>() << std::endl;
}