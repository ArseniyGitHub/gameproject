#pragma once
#include  <vector>
#include  <string>
#include <iostream>
#include "types.h"
#include   <SFML/Graphics.hpp>

struct ParserElem {
	enum Types {
		coords,
		rotation,
		speed,
		direction,
		brightness,
		id,
		color,
		end
	};
	Types   type;
	void*   data;
	template <typename T> ParserElem(Types type, T* data, bool cpy = true) : type(type) {
		if (cpy) {
			this->data = new T;
			*(T*)(this->data) = *data;
		}
		else this->data = data;
	}
	ParserElem() {

	}
};

class Parser {
public:
	struct ErrorType {};
	static std::vector<ParserElem> unPack(const ui8* data, const ui64 size) {
		std::vector<ParserElem> ret;
		ui64 counter = 0;
		ui64 _sz = 0;
		ParserElem::Types T = ParserElem::Types::end;
		for (ui64 i = 0; i < size;) {


			T = *(ParserElem::Types*)(data + i);  i += sizeof(ParserElem::Types);
			if (T >= ParserElem::Types::end) throw ErrorType();

			switch (T) {
			case ParserElem::Types::coords:
				_sz = 2 * 4;
				break;
			case ParserElem::Types::rotation:
				_sz = 4;
				break;
			case ParserElem::Types::speed:
				_sz = 4;
				break;
			case ParserElem::Types::direction:
				_sz = 2 * 4;
				break;
			case ParserElem::Types::brightness:
				_sz = 4;
				break;
			case ParserElem::Types::color:
				_sz = 4;
				break;
			case ParserElem::Types::id:
				_sz = 4;
				break;
			}
			ParserElem obj;
			obj.data = new ui8[_sz];
			ui8* ptr = (ui8*)(obj.data);
			counter =  0;
			obj.type = T;
			while (counter < _sz) {
				std::cout << (int)data[i] << std::endl;
				ptr[counter++] = data[i++];
			}
			ret.push_back(obj);
		}
		return ret;
	}

	static std::vector<ui8> pack(ParserElem& frst) { 
		ui64 sz = 0;   ui8* writeData = (ui8*)frst.data;
		switch (frst.type) {
		case ParserElem::Types::coords:
			sz = 2 * 4;
			break;
		case ParserElem::Types::rotation:
			sz = 4;
			break;
		case ParserElem::Types::speed:
			sz = 4;
			break;
		case ParserElem::Types::direction:
			sz = 2 * 4;
			break;
		case ParserElem::Types::brightness:
			sz = 4;
			break;
		case ParserElem::Types::color:
			sz = 4;
			break;
		case ParserElem::Types::id:
			sz = 4;
			break;
		}
		std::vector<ui8> ret(sizeof(ParserElem::Types) + sz);
		for (ui64 i = 0; i < sizeof(ParserElem::Types); i++) {
			ret[i] = ((ui8*)(&frst.type))[i];
		}
		for (ui64 i = 0; i < sz; i++) {
			ret[i + sizeof(ParserElem::Types)] = writeData[i];
		}
		return ret;
	}
	/*
    static std::vector<ui8> pack(ParserElem frst, ParserElem... args) {
		std::vector<ui8> ret = pack(frst);
		std::vector<ui8>& inc = pack(args...);
		ret.insert(ret.end(), inc.begin(), inc.end());
		//ret.emplace_back(pack(any...));
		return ret;
	}
	*/
	static std::vector<ui8> pack(std::vector<ParserElem>& data) {
		std::vector<ui8> ret;
		for (auto& el : data) {
			std::vector<ui8>& inc = pack(el);
			ret.insert(ret.end(), inc.begin(), inc.end());
		}
		return ret;
	}
};