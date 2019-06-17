#include "lee.h"
#include <iostream>
namespace srb {
	namespace trans {
		char u8ToLow(uint8 u8) {
			char c = '0' + (u8 & 0x0f);
			if (c > '9') { c = c - '9' - 1 + 'A'; }
			return c;
		}
	}
};