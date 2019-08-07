#pragma once
#include "lee.h"
#include <string>


namespace lee8871_support {

	class LString {
	protected:
		char* buf;
		const size_t size;
		size_t ptr_i = 0;
	public:
		LString(char *_buf, int _size);
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
		}
		int append(const char* a);
		int append(char append);
		int printf(const char *format, ...);
	};



	enum class eJsonWarning {
		get_no_num = -201,
		get_float_to_int = -202,
		get_other_string = -203,
		get_negative_to_unsigned = -204,
		overflow = -204,

	};
	class JsonString : public LString {
	protected:
		int tab_level = 0;
	public:

		bool isExpanded = true;
		char* tab_string = "  ";
		JsonString(char *_buf,int _size);

		inline void reset() {
			LString::reset();
			tab_level = 0;	
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

		int outputNumber(int* value);
		int outputNumber(unsigned int * value);
		int outputNumber(float* value);
		int outputNumber(double* value);
		
		int inputBool(bool value);
	};


};