#include <cstdlib>
#include <cmath>

#include "JsonParseString.h"
#define NOT_JSON_USER
#include "json.h"

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
		bool is_overflow = (size == 0);
		char * str_end = str + size;
		if (checkCh('"')==false) {
			captureError("J2N")->append("String should start with '\"'.");
			return eJsonParse.get_not_string;
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
					break;
				}
				_ptr++;
			}
			else if (*_ptr == '"') {
				_ptr++;
				checkAppendString(0);
				if ((is_overflow)&&(size!=0)) {
					captureError("J2S")->append("String buffer is too small for string.");
					return eJsonParse.overflow;
				}
				else {
					return done;
				}
			}	
			else if ((*_ptr < 0x20)|| (*_ptr >=0x7f)) {
				captureError("J2N")->print("Unknow control char 0x%x", *_ptr);
				return eJsonParse.get_cc_unpaired_DQM;
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

	int JsonParseString::parseNumber(int* value){
		outputRemoveSpace();
		char* p_end;
		*value = strtol(_ptr, &p_end, 10);
		if (p_end == _ptr){
			captureError("J2I")->append("Object is not a number.\n");
			return eJsonParse.no_a_num;
		}
		else if ((*p_end == 'e') || (*p_end == '.')) {
			auto d=strtold(_ptr, &p_end);
			_ptr = p_end;
			*value = (int)round(d);
			if ((*value == INT_MAX) || (*value == INT_MIN)) {
				captureError("J2I")->append("Object is float but transport to int and may overflow.\n");
				return eJsonParse.float_to_int_overflow;
			}
			else {
				get_errorString()->append("Object may be longer than int. ");
				return eJsonParse.float_to_int;
			}
		}
		else {
			_ptr = p_end;
			if ((*value == INT_MAX) || (*value == INT_MIN)) {
				captureError("J2I")->append("Object may be longer than int.\n");
				return eJsonParse.overflow;
			}
			else {				
				return done;
			}
		}
	}
	int JsonParseString::parseNumber(unsigned int * value) {
		return done;
	}
	int JsonParseString::parseNumber(float* value) {
		return done;
	}
	int JsonParseString::parseNumber(double* value) {
		char* p_end;
		*value = strtold(_ptr, &p_end);
		if (p_end == _ptr){
			return eJsonParse.no_a_num;
		}
		return done;
	}
	



};