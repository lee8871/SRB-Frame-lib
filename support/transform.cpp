
#include <iostream>
#include <time.h> 
#include "lee.h"
#include "transform.h"
namespace srb {
	namespace trans {
		char u8ToLow(uint8 u8) {
			char c = '0' + (u8 & 0x0f);
			if (c > '9') { c = c - '9' - 1 + 'A'; }
			return c;
		}
		int usTotimestr(char* out_str, int len, tUs us_time) {
			time_t tim = us_time / 1000000;
			return strftime(out_str, len, "%Y%m%d-%H-%M-%S", localtime(&tim));
		}
	}
};