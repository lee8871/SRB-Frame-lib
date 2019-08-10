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
		LString* const& Error_str = error_str;
		bool isExpanded = true;
		char* tab_string = "  ";
		JsonString(char *buf, int size);
		JsonString(int size);
		~JsonString();



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
		int outputString(char * s, int size);
		int abondonString();

		int inputNumber(int value);
		int inputNumber(unsigned int value);
		int inputNumber(double value);
		int inputNumber(float value);

		int outputNumber(int* value);
		int outputNumber(unsigned int * value);
		int outputNumber(float* value);
		int outputNumber(double* value);

		void outputRemoveSpace();
		bool confirm(char c);		
		int inputBool(bool value);




		LString* get_errorString();
		LString*  reportError(const char * type) {
			if (error_str == nullptr) {
				error_str = new LString(ERROR_CP_SIZE);
			}
			error_str->append(type);
			if (Ptr - 10 >= Buf) {
				error_str->append(":...");
				error_str->append(Ptr - 10, 20);
				error_str->append("\n");
			}
			else{
				error_str->append(":");
				error_str->append(Buf, 20);
				error_str->append("\n");
			}
			return error_str;
		}


	};


};