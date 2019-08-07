#include "JsonString.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>

namespace lee8871_support {
	#define addAndCheck(a) *(ptr++)= (a);if (ptr == end) { *(ptr - 1) = 0;return buf_use_up; }
	#define endString() *ptr = 0; return done;
	LString::LString(char *_buf, int _size) :
		end(_buf + _size), buf(_buf), ptr(_buf) {}
	
	int LString::append(char a) {
		addAndCheck(a);
		endString();
	}		
	void LString::reset() {
		ptr = buf;
	}

	int LString::append(const char* a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int increase = vsnprintf(ptr, end - ptr, format, args);
		va_end(args);		
		if (increase < 0) {
			return -210 + increase;
		}
		ptr += increase;
		if (ptr == end) {
			return buf_use_up;
		}
		return done;
	}

	int JsonString::inputString(const char * string)	{
		addAndCheck('"');
		while (*string != 0){
			switch (*string) {
			case '"':
				addAndCheck('\\');addAndCheck('"');
				break;
			case '\\':
				addAndCheck('\\');addAndCheck('\\');
				break;
			case '/':
				addAndCheck('\\');addAndCheck('/');
				break;
			case '\b':
				addAndCheck('\\');addAndCheck('b');
				break;
			case '\f':
				addAndCheck('\\');addAndCheck('f');
				break;
			case '\n':
				addAndCheck('\\');addAndCheck('n');
				break;
			case '\r':
				addAndCheck('\\');addAndCheck('r');
				break;
			case '\t':
				addAndCheck('\\');addAndCheck('t');
				break;
			default:
				addAndCheck(*string);
				//TODO /u is not support now
			}
			string++;
		}
		addAndCheck('"');
		endString();
	}

	int JsonString::inputNumber(int value) {
		return print("%d", value);
	}	
	int JsonString::inputNumber(unsigned int value) {
		return print("%u", value);
	}
	int JsonString::inputNumber(double value) {
		return print("%lf", value);
	}
	int JsonString::inputNumber(float value) {
		return print("%f", value);
	}	
	int JsonString::inputBool(bool value) {
		if (value) {
			return append("true");
		}
		else {
			return append("false");
		}
	}

	int JsonString::objectBgn() {
		addAndCheck('{');
		if (isExpanded) {
			addAndCheck('\n');
			tab_level++;
			for (int i = 0;i < tab_level;i++) {
				checkFailReturn(append(tab_string));
			}
		}
		endString();
	}
	int JsonString::objectGasket() {
		addAndCheck(',');
		if (isExpanded) {
			addAndCheck('\n');
			for (int i = 0;i < tab_level;i++) {
				checkFailReturn(append(tab_string));
			}
		}
		endString();
	}
	int JsonString::objectEnd() {
		if (isExpanded) {
			addAndCheck('\n');
			tab_level--;
			for (int i = 0;i < tab_level;i++) {
				checkFailReturn(append(tab_string));
			}
		}
		addAndCheck('}');
		endString();
	}
	int JsonString::arrayBgn() {
		addAndCheck('[');
		endString();

	}
	int JsonString::arrayGasket() {
		addAndCheck(',');
		endString();
	}
	int JsonString::arrayEnd() {
		addAndCheck(']');
		endString();

	}

	JsonString::JsonString(char *_buf, int _size) 
		:LString(_buf,_size){
		tab_level = 0;
	}

#undef addAndCheck 
#undef endString 


	int JsonString::outputNumber(int* value){
		char* p_end;
		*value = strtol(ptr, &p_end, 10);
		if (p_end == ptr){
			return (int)eJsonWarning::get_no_num;
		}
		else if ((*p_end == 'e') || (*p_end == '.')) {
			auto d=strtold(ptr, &p_end);
			ptr = p_end;
			*value = round(d);
			//TODO maybe is integer like 23e3
			return (int)eJsonWarning::get_float_to_int;
		}
		else {
			ptr = p_end;
			if ((*value == INT_MAX) || (*value == INT_MIN)) {
				return (int)eJsonWarning::overflow;
			}
			else {				
				return done;
			}
		}
	}
	int JsonString::outputNumber(unsigned int * value) {

		return done;
	}
	int JsonString::outputNumber(float* value) {

		return done;
	}
	int JsonString::outputNumber(double* value) {
		char* p_end;
		*value = strtold(ptr, &p_end);
		if (p_end == ptr){
			return -202;
		}
		return done;
	}



};