#include <cstdlib>
#include <cmath>

#include "JsonString.h"
#include "json.h"

namespace lee8871_support {
	#define addAndCheck(a) *(_ptr++)= (a);if (_ptr == End) { *(_ptr - 1) = 0;return buf_use_up; }
	#define endString() *_ptr = 0; return done;

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
	#define checkAppendString(ch)do{\
		if (is_overflow == false){\
			*str++ = ch;\
			if (str >= str_end){\
				(--str) = 0; \
				is_overflow = true;\
			}\
		}\
	}while(0)//eRevJson.overflow;

	int JsonString::outputString(char * str, int size) {
		bool is_overflow = (size == 0);

		char * str_end = str + size;

		if (confirm('"')==false) {
			return eRevJson.get_not_string;
		}
		while (*str != 0) {
			if (*_ptr == '\\'){
				_ptr++;
				switch (*_ptr) {
				case '"':
					checkAppendString('"');
					break;
				case '\\':
					checkAppendString('\\');
					break;
				case '/':
					checkAppendString('/');
					break;
				case 'b':
					checkAppendString('\b');
					break;
				case 'f':
					checkAppendString('\f');
					break;
				case 'n':
					checkAppendString('\n');
					break;
				case 'r':
					checkAppendString('\r');
					break;
				case 't':
					checkAppendString('\t');
					break;
				default:	
					break;
				}
				_ptr++;
			}
			else if (*_ptr == '"') {
				_ptr++;
				checkAppendString(0);
				return done;
			}
			else {
				checkAppendString(*_ptr);
				_ptr++;
			}
		}	
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

	JsonString::JsonString(int size)
		: LString(size) {
		tab_level = 0;
	}

	JsonString::~JsonString(){
		if (error_str != nullptr) {
			delete error_str;
		}
	}

	LString * JsonString::get_errorString(){
		if (error_str == nullptr) {
			error_str = new LString(ERROR_CP_SIZE);
		}
		return error_str;
	}

#undef addAndCheck 
#undef endString 


	int JsonString::outputNumber(int* value){
		outputRemoveSpace();
		char* p_end;
		*value = strtol(_ptr, &p_end, 10);
		if (p_end == _ptr){
			return eRevJson.get_no_num;
		}
		else if ((*p_end == 'e') || (*p_end == '.')) {
			auto d=strtold(_ptr, &p_end);
			_ptr = p_end;
			*value = (int)round(d);
			//TODO maybe is integer like 23e3
			return eRevJson.get_float_to_int;
		}
		else {
			_ptr = p_end;
			if ((*value == INT_MAX) || (*value == INT_MIN)) {
				return eRevJson.overflow;
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
		*value = strtold(_ptr, &p_end);
		if (p_end == _ptr){
			return eRevJson.get_no_num;
		}
		return done;
	}

	void JsonString::outputRemoveSpace() {
		while (_ptr != End) {
			if ((*_ptr == ' ') || (*_ptr == '\r') ||
				(*_ptr == '\t') || (*_ptr == '\n')) {
				_ptr++;
			}
			else {
				return;
			}
		}
	}

	bool JsonString::confirm(char c)	{
			outputRemoveSpace();
		if (*_ptr == c) {
			_ptr++;
			return true;
		}
		return false;
	}

	



};