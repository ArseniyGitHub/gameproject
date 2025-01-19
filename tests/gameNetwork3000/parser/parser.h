#pragma once
#include  <vector>
#include  <string>
#include <iostream>
#include "types.h"
#include <SFML/Graphics.hpp>

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



using __bytes = std::vector<ui8>;

template <typename nameType, typename typeType, typename blockType, typename sizeType>
struct __defParserVer;

struct __NullParserType {
	__NullParserType(){}
	template <typename type> __NullParserType(type& from){}
	__bytes* parse() {
		return nullptr;
	}
	void unParse(__bytes* pack) {
		return;
	}
};

template <typename _Type>
struct __ParserByteCopy {
	_Type el;
	__ParserByteCopy(__ParserByteCopy& from) : el(from.el) {}
	__ParserByteCopy(_Type from) : el(from) {}
	__ParserByteCopy() {}
	void unParse(__bytes* _from) {
		__bytes& from = *_from;
		for (ui64 i = 0; i < sizeof(_Type) && i < from.size(); i++) {
			((ui8*)(&el))[i] = from[i];
		}
		from.erase(from.begin(), from.begin() + sizeof(_Type));
	}
	__bytes* parse() {
		__bytes* ret = new __bytes(sizeof(_Type));
		for (ui64 i = 0; i < sizeof(_Type); i++) {
			(*ret)[i] = ((ui8*)(&el))[i];
		}
		return ret;
	}
};

template <typename _sizeType>
struct __ParserText {
	using sizeType = __ParserByteCopy<_sizeType>;
	std::string text;
	__ParserText(__ParserText& from) : text(from.text) {}
	__ParserText() {}
	__bytes* parse() {
		__bytes* boofer;  __bytes bf2(text.size());
		sizeType sz = text.size();

		boofer = sz.parse();
		//if (boofer != nullptr && !boofer->empty()) ret.insert(ret.end(), boofer->begin(), boofer->end());
		//delete boofer;
		if (boofer == nullptr) boofer = new __bytes;

		for (ui64 i = 0; i < text.size(); i++) {
			bf2[i] = text[i];
		}
		boofer->insert(boofer->end(), bf2.begin(), bf2.end());
		return boofer;
	}

	void unParse(__bytes* _from) {
		text.clear();
		__bytes& from = *_from;
		sizeType sz;
		sz.unParse(_from);
		text.resize(sz.el);
		for (ui64 i = 0; i < sz.el && i < from.size(); i++) {
			this->text[i] = from[i];
		}
		from.erase(from.begin(), from.begin() + sz.el);
	}
};

struct __ParserBlock {
	__bytes data;
	__bytes* parse() {
		__bytes* ret = new __bytes;
		ret->insert(ret->begin(), data.begin(), data.end());
		return ret;
	}
	void unParse(__bytes* _from, ui64 sz) {
		__bytes& from = *_from;
		data.clear();
		data.insert(data.begin(), from.begin(), from.begin() + sz);
		from.erase(from.begin(), from.begin() + sz);
	}
};

template <typename nameType, typename typeType, typename blockType, typename sizeType>
struct __defParserVer {
	nameType name;   sizeType size;  typeType type;  blockType data;
	fn<ui64, sizeType&, typeType&> _Unparse_Process = nullptr;
	__defParserVer(sizeType _size, nameType _name, typeType _type, blockType _data) : size(_size), name(_name), type(_type), data(_data) {}
	__defParserVer() {}
	__bytes* parse() {
		__bytes& ret = *(new __bytes);
		__bytes* boofer;
		boofer = (name.parse());
		if (boofer != nullptr && !boofer->empty()) ret.insert(ret.end(), boofer->begin(), boofer->end());
		delete boofer;

		boofer = (type.parse());
		if (boofer != nullptr && !boofer->empty()) ret.insert(ret.end(), boofer->begin(), boofer->end());
		delete boofer;

		boofer = (size.parse());
		if (boofer != nullptr && !boofer->empty()) ret.insert(ret.end(), boofer->begin(), boofer->end());
		delete boofer;

		boofer = (data.parse());
		if (boofer != nullptr && !boofer->empty()) ret.insert(ret.end(), boofer->begin(), boofer->end());
		delete boofer;

		return &ret;
	}
	void unParse(__bytes* _from) {
		__bytes& from = *_from;
		name.unParse(_from);
		type.unParse(_from);
		size.unParse(_from);
		ui64 sz = 0;
		if (_Unparse_Process != nullptr) sz = _Unparse_Process(size, type);
		else sz = size.el;
		data.unParse(_from, sz);
	}
};

/*
template <typename variableT>
class _ParserV2 {
	static __bytes parse(std::vector<variableT> input) {
		__bytes ret;   __bytes boofer;
		for (variableT& el : input) {
			boofer = el.parse();
			ret.insert(ret.end(), boofer.begin(), boofer.end());
		}
		return ret;
	}
	static std::vector<variableT> unParse(__bytes input) {
		std::vector<variableT> ret;
		while (!ret.empty()) {
			ret.push_back(variableT::unParse(input));
		}
		return ret;
	}
};
*/