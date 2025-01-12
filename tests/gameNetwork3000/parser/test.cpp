#include "parser.h"
#include <iostream>

int main() {
	std::vector<ParserElem> test = { ParserElem(ParserElem::coords, &sf::Vector2f(9, 11)), ParserElem(ParserElem::speed, &num<float>(4))};
	std::vector<ui8> ret = Parser::pack(test);
	std::vector<ParserElem> out = Parser::unPack(ret.data(), ret.size());
	std::cout << "type: " << out[0].type << ", data: " << ((sf::Vector2f*)out[0].data)->x << "   " << ((sf::Vector2f*)out[0].data)->y << std::endl;
	std::cout << "type: " << out[1].type << ", data: " << *((float*)out[1].data) << std::endl;
}