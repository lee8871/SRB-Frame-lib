#include "JsonString.h"
#include <stdarg.h>
#include <stdio.h>


namespace lee8871_support {
#define addAndCheck(a) buf[ptr_i++] = (a);if (ptr_i == size) { buf[ptr_i - 1] = 0;return fail; }
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
				addAndCheck('\\');
				addAndCheck('"');
				break;
			case '\\':
				addAndCheck('\\');
				addAndCheck('\\');
				break;
			case '/':
				addAndCheck('\\');
				addAndCheck('/');
				break;
			case '\b':
				addAndCheck('\\');
				addAndCheck('b');
				break;
			case '\f':
				addAndCheck('\\');
				addAndCheck('f');
				break;
			case '\n':
				addAndCheck('\\');
				addAndCheck('n');
				break;
			case '\r':
				addAndCheck('\\');
				addAndCheck('r');
				break;
			case '\t':
				addAndCheck('\\');
				addAndCheck('t');
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
		double print_inc = snprintf(ptr(), rem(), "%lf", value);
		return JsonString::inc(print_inc);
	}
	int JsonString::inputNumber(float value) {
		float print_inc = snprintf(ptr(), rem(), "%f", value);
		return JsonString::inc(print_inc);
	}

#undef addAndCheck 
#undef endString 

};