#include "parser.h"
#include <iostream>

int main() {
	std::vector<ParserElem> test = { ParserElem(ParserElem::coords, &sf::Vector2f(9, 11)), ParserElem(ParserElem::speed, &num<float>(4)) };
	std::vector<ui8> ret = Parser::pack(test);
	for (auto& el : ret) {
		std::cout << (ui64)el;
	}
}