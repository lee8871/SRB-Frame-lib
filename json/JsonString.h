#pragma once
#include "lee.h"
#include <string>


namespace lee8871_support {

	class LString {
	protected:
		char* buf;
		const size_t size;
		size_t ptr_i = 0;
		LString(char *_buf, int _size);
	public:
		inline char* ptr() {
			return buf + ptr_i;
		}
		inline int inc(int increase) {
			if (increase < 0) {
				return -100 + increase;
			}
			ptr_i += increase;
			if (ptr_i == size) {
				return buf_use_up;
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
		int append(const char* a);
		int append(char append);

		int printf(const char *format, ...);


	};

	class JsonString : public LString {
	protected:
		int tab_level = 0;
		char* error_str = nullptr;
	public:

		bool isExpanded = true;
		char* tab_string = "  ";


		JsonString(char *_buf,int _size);	
		char* errorReport(int length = 1024);

		inline void reset() {
			LString::reset();
			tab_level = 0;		
			if (error_str != nullptr) {
				delete[] error_str;
				error_str = nullptr;
			}
		}
		int objectBgn();
		int objectGasket();
		int objectEnd();
		int arrayBgn();
		int arrayGasket();
		int arrayEnd();

		int inputString(const char * string);

		int inputNumber(int value);
		int inputNumber(unsigned int value);
		int inputNumber(double value);
		int inputNumber(float value);

		int inputBool(bool value);
	};


};