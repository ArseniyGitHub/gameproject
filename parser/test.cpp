#include "netparser.h"
#include "ParserAPI.hpp"
#include <iostream>
#include <map>

//using namespace Parser;

int main() {
	
	Parser<ui32> a;
	a["msg"] = (i64)9;
	a["helo"]["idk"] = 3.6f;
	a["helo"]["haha"] = "amogus";
	a["helo"].setName("hello");
	a["hello"][0].getAs<double>() -= 5;
	a["hello"]["haha"][0] = "sus";

	__bytes* data = a.pack();

	Parser<ui32> b;
	b.parse(data);

	std::cout << b["hello"]["idk"].getAs<double>() << "   " << b["hello"]["haha"][0].getAs<std::string>() << std::endl;
}