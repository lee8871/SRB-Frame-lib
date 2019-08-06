#pragma once
#include "lee.h"


namespace lee8871_support {
	class LString {
	private:
		char* buf;
		size_t ptr_i = 0;
		size_t size;
	public:
		LString(char* b, int s) :
			buf(b), size(s) {
			buf[size - 1] = 0;
		}
		inline char* ptr() {
			return buf + ptr_i;
		}
		inline int inc(int increase) {
			if (increase < 0) {
				return -100 + increase;
			}
			ptr_i += increase;
			if (ptr_i == size) {
				return fail;
			}
			return done;
		}
		inline size_t rem() {
			return (size - ptr_i);
		}
		inline void reset() {
			ptr_i = 0;
			buf[ptr_i] = 0;
		}
		int append(char* append);
		int append(char append);
		int printf(const char *format, ...);
	};
	constexpr unsigned int NO_HASH = 0;
	unsigned int getHashString(const char * str);
	unsigned int getHashByDoubleQuotes(const char * str);


};