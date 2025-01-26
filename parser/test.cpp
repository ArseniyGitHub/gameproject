#include "parser.h"
#include <iostream>
#include <map>

ui64 getSize(__ParserByteCopy<ui64>& s) {
	switch (s.el) {
	case 3:
		return 4;
		break;
	case 5:
		return 3;
		break;
	}
}

int main() {
	
	Parser2_Eco<ui64, getSize> a;
	a[0].type = 3;
	a[0].data.data = *__ParserByteCopy<ui32>(95).parse();

	Parser2_Eco<ui64, getSize> b;
	b.unParse(a.parse());
	std::cout << b[0].type.el << "   " << __ParserByteCopy<ui32>().unParse(&(b[0].data.data)).el << "   " << b[0].size.el << std::endl;
}