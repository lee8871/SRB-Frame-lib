#pragma once
#include "lee.h"
#include "LString.h"


namespace lee8871_support {

	class JsonString : public LString {
	protected:
		int tab_level = 0;
		constexpr static int ERROR_CP_SIZE = 4096;
		LString* error_str = nullptr;
	public:
		bool isExpanded = true;
		char* tab_string = "  ";
		JsonString(char *_buf,int _size);
		~JsonString();


		LString* get_errorString();

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
		bool confirm(char c);
		bool confirm(char c1, char c2);
		bool confirm(char c1, char c2, char c3);
		bool confirmNumber();
		
		int inputBool(bool value);
	};


};