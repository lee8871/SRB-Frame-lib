#include "JsonString.h"
#include <stdarg.h>
#include <stdio.h>


namespace lee8871_support {
#define addAndCheck(a) buf[ptr_i++] = (a);if (ptr_i == size) { buf[ptr_i - 1] = 0;return buf_use_up; }
#define endString() buf[ptr_i] = 0; return done;


	int JsonString::append(char a) {
		addAndCheck(a);
		endString();
	}
	int JsonString::printf(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int print_inc = vsnprintf(ptr(), rem(), format, args);
		va_end(args);
		return inc(print_inc);
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
		int print_inc = snprintf(ptr(), rem(), "%d", value);
		return JsonString::inc(print_inc);
	}

	int JsonString::inputNumber(unsigned int value) {
		int print_inc = snprintf(ptr(), rem(), "%u", value);
		return JsonString::inc(print_inc);
	}

	int JsonString::inputNumber(double value) {
		int print_inc = snprintf(ptr(), rem(), "%lf", value);
		return JsonString::inc(print_inc);
	}
	int JsonString::inputNumber(float value) {
		int print_inc = snprintf(ptr(), rem(), "%f", value);
		return JsonString::inc(print_inc);
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
				addAndCheck('\t');
			}
		}
		endString();
	}
	int JsonString::objectGasket() {
		addAndCheck(',');
		if (isExpanded) {
			addAndCheck('\n');
			for (int i = 0;i < tab_level;i++) {
				addAndCheck('\t');
			}
		}
		endString();
	}
	int JsonString::objectEnd() {
		if (isExpanded) {
			addAndCheck('\n');
			tab_level--;
			for (int i = 0;i < tab_level;i++) {
				addAndCheck('\t');
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

	int JsonString::append(const char* a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}




#undef addAndCheck 
#undef endString 

};