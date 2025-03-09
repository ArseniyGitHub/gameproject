#pragma once
#include "parser.h"
#include <list>
#include <memory>

using namespace Parser;

class Parser {
	__ParserByteCopy<ui32> elemsCount = 0;
	std::vector<__defParserVer<__ParserByteCopy<ui32>, __ParserByteCopy<ui32>, __ParserBlock, __ParserByteCopy<ui64>>> elems;
	
	enum standardTypes {
		_elems, 
	    _int,
		_float, 
		_string,
		end
	};
	
	/// <summary>
	/// не гарантируем безопасность ваших личных данных!!!
	/// </summary>
	/// <param name="from"></param>
	void unParse(__bytes* from) {
		elems.clear();
		elemsCount.unParse(from);
		elems.resize(elemsCount);
		for (ui32 i = 0; i < elemsCount; i++) {
			elems[i].unParse(from);
		}
	}

	std::shared_ptr<__bytes> parse() {
		__bytes* ret = new __bytes;
		__bytes* buffer;
		buffer = elemsCount.parse();
		ret->insert(ret->end(), buffer->begin(), buffer->end());
		delete buffer;
		for (ui32 i = 0; i < elemsCount; i++) {

			if (elems[i].type < standardTypes::end) {
				switch (elems[i].type) {
				case standardTypes::_float:
					if (elems[i].bufferSize == 4) {
						elems[i].data.data = *(__ParserByteCopy<float>(*(float*)elems[i].elemBoofer).parse());
					}
					else if (elems[i].bufferSize == 8) {
						elems[i].data.data = *(__ParserByteCopy<double>(*(double*)elems[i].elemBoofer).parse());
					}
					break;
				}
			}

			buffer = elems[i].parse();
			ret->insert(ret->end(), buffer->begin(), buffer->end());
			delete buffer;
		}
		return ret;
	}

	
};