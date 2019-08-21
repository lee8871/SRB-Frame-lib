#include <cstdlib>
#include <cmath>

#include "JsonParseString.h"
#include "Json.in.h"

namespace lee8871_support {


	#define checkAppendString(ch)do{\
		if (is_overflow == false){\
			*(str++) = ch;\
			if (str >= str_end){\
				*(--str) = 0; \
				is_overflow = true;\
			}\
		}\
	}while(0)

	int JsonParseString::parseString(char * str, int size) {
		recordBeginPtr();
		bool is_overflow = (size == 0);
		char * str_end = str + size;
		if (checkCh('"')==false) {
			captureAndPrintError("String should start with '\"'.\n");
			_last_alram = eAlram::not_str;
			return fail;
		}
		while (1) {
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
					ERROR("\\u or other escape is not support.");
					break;
				}
				_ptr++;
			}
			else if (*_ptr == '"') {
				_ptr++;
				checkAppendString(0);
				if ((is_overflow)&&(size!=0)) {
					captureAndPrintError("String buffer is too small for string.\n");
					_last_alram = eAlram::str_overflow;
					return done;
				}
				else {
					return done;
				}
			}	
			else if ((*_ptr < 0x20)|| (*_ptr >=0x7f)) {
				captureAndPrintError("Unknow control char 0x%x\n", *_ptr);
				_last_alram = eAlram::str_token_unpair;
				return done;
			}
			else {
				checkAppendString(*_ptr);
				_ptr++;
			}
		}
	}
	JsonParseString::JsonParseString(char *_buf, int _size)
		:LString(_buf,_size){
		tab_level = 0;
	}
	JsonParseString::JsonParseString(int size)
		: LString(size) {
		tab_level = 0;
	}
	JsonParseString::~JsonParseString(){
		if (error_str != nullptr) {
			delete error_str;
		}
	}
	LString * JsonParseString::get_errorString(){
		if (error_str == nullptr) {
			error_str = new LString(ERROR_CP_SIZE);
		}
		return error_str;
	}

	int JsonParseString::parseNumber(int* val){
		recordBeginPtr();
		outputRemoveSpace();
		char* p_end;
		int temp = strtol(_ptr, &p_end, 10);
		if (p_end == _ptr){
			captureAndPrintError("Jsonstring is not a number.\n");
			_last_alram = eAlram::no_a_num;
			return fail;
		}
		else if ((*p_end == 'e') || (*p_end == '.')) {
			auto d=strtold(_ptr, &p_end);
			_ptr = p_end;
			if ((d < INT_MIN) || (d > INT_MAX)) {
				captureAndPrintError("Number is float but transport to int and overflow.\n");
				_last_alram = eAlram::float_to_int_overflow;
				*val = (int)round(d);
				return done;
			}
			else {
				captureAndPrintError("Number is float but transport to int \n");
				_last_alram = eAlram::float_to_int;
				*val = (int)round(d);
				return done;
			}
		}
		else {
			_ptr = p_end;
			if ((temp == INT_MAX) || (temp == INT_MIN)) {
				captureAndPrintError("Number transport to int overflow.\n");
				_last_alram = eAlram::overflow;
				*val = temp;
				return done;
			}
			else {
				*val = temp;
				return done;
			}
		}
	}
	int JsonParseString::parseNumber(unsigned int * value) {
		recordBeginPtr();
		outputRemoveSpace();
		char* p_end;
		if (checkCh('-')) {
			auto d = strtold(_ptr, &p_end);
			_ptr = p_end;
			captureAndPrintError("Number is a negative but transform to unsigned. \n");
			_last_alram = eAlram::negative_to_unsigned;
			*value =0;
			return done;

		}
		auto temp = strtoul(_ptr, &p_end, 10);
		if (p_end == _ptr) {
			captureAndPrintError("Jsonstring is not a number.\n");
			_last_alram = eAlram::no_a_num;
			return fail;
		}
		else if ((*p_end == 'e') || (*p_end == '.')) {
			auto d = strtold(_ptr, &p_end);
			_ptr = p_end;
			if ((d < 0) || (d > UINT_MAX)) {
				captureAndPrintError("Number is float but transform to unsigned int and overflow.\n");
				_last_alram = eAlram::float_to_int_overflow;
				*value = UINT_MAX;	return done;
			}
			else {
				captureAndPrintError("Number is float but transform to unsigned int.\n");
				_last_alram = eAlram::float_to_int;
				*value = (unsigned int)round(d);	return done;
			}
		}
		else {
			_ptr = p_end;
			if (*value == UINT_MAX) {
				captureAndPrintError("Number transport to unsigned int overflow.\n");
				_last_alram = eAlram::overflow;
				*value = temp;	return done;
			}
			else {
				*value = temp;	return done;
			}
		}
	}
	int JsonParseString::parseNumber(float* value) {
		recordBeginPtr();
		outputRemoveSpace();
		char* p_end;
		auto temp = strtof(_ptr, &p_end);
		if (p_end == _ptr) {
			captureAndPrintError("Object is not a number.\n");
			_last_alram = eAlram::no_a_num;
			return fail;
		}
		_ptr = p_end;
		*value = temp;
		return done;
	}
	int JsonParseString::parseNumber(double* value) {
		recordBeginPtr();
		outputRemoveSpace();
		char* p_end;
		auto temp = strtold(_ptr, &p_end);
		if (p_end == _ptr){
			captureAndPrintError("Object is not a number.\n");
			_last_alram = eAlram::no_a_num;
			return fail;
		}
		_ptr = p_end;
		*value = temp;
		return done;
	}

};