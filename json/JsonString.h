#pragma once
#include "lee.h"


namespace lee8871_support {
	class JsonString {
	private:
		char* buf;
		size_t ptr_i = 0;
		size_t size;
	public:
		JsonString(char* b, int s) :
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
		int append(char append);
		int printf(const char *format, ...);
		int inputString(const char * string);

		int inputNumber(int value);
		int inputNumber(unsigned int value);
		int inputNumber(double value);
		int inputNumber(float value);
	};


};