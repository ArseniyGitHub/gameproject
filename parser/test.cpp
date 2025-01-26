#include "parser.h"
#include <iostream>
#include <map>

int main() {
	
	Parser2<__ParserByteCopy<ui64>> a;
	a[0].name.text = "hello!";
	a[0].type.el = 87;
	a[0].size.el = 4;
	a[0].data.data = *__ParserText<ui32>(std::string("hello!")).parse();
	__bytes& b = *a.parse();

	Parser2<__ParserByteCopy<ui64>> c;
	c.unParse(&b);
	std::cout << c[0].name.text << std::endl;
}