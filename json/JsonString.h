#pragma once
#include "lee.h"
#include <string>


namespace lee8871_support {

	class LString {
	private:
		char* const buf;
	protected:
		char* const end;
		char* ptr;
	public:
		LString(char *_buf, int _size);
		void reset();
		int append(const char* a);
		int append(char append);
		int print(const char *format, ...);
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

		int outputRemoveSpace();

		
		int inputBool(bool value);
	};


};