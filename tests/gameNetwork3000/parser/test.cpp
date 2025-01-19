#include "parser.h"
#include <iostream>
#include <map>

int main() {
	/*
	std::vector<ParserElem> test = { ParserElem(ParserElem::coords, &sf::Vector2f(9, 11)), ParserElem(ParserElem::speed, &num<float>(4))};
	std::vector<ui8> ret = Parser::pack(test);
	std::vector<ParserElem> out = Parser::unPack(ret.data(), ret.size());
	std::cout << "type: " << out[0].type << ", data: " << ((sf::Vector2f*)out[0].data)->x << "   " << ((sf::Vector2f*)out[0].data)->y << std::endl;
	std::cout << "type: " << out[1].type << ", data: " << *((float*)out[1].data) << std::endl;
	*/
	

	__defParserVer<__ParserText<ui64>, __ParserByteCopy<ui64>, __ParserBlock, __ParserByteCopy<ui64>> a;
	a.name.text = "myVariable";
	a.type.el = 3;
	a.size = 4;
	a.data.data = *__ParserByteCopy<ui32>(98).parse();
	__defParserVer<__ParserText<ui64>, __ParserByteCopy<ui64>, __ParserBlock, __ParserByteCopy<ui64>> b;
	b.unParse(a.parse());
	std::cout << "name: " << b.name.text << ", type: " << a.type.el << ", size: " << a.size.el << std::endl;
}