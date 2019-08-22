#include <cstdlib>
#include <cmath>
#include <limits.h>

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


	//关于报错
	void JsonParseString::recordBeginPtr() {
		sign_begin = Ptr;
	}
	LString * JsonParseString::get_errorString(){
		if (error_str == nullptr) {
			error_str = new LString(ERROR_CP_SIZE);
		}
		return error_str;
	}
	LString * JsonParseString::captureError(const char * type) {
		if (error_str == nullptr) {
			error_str = new LString(ERROR_CP_SIZE);
		}
		const char* sign_end = Ptr;
		error_str->print("[%s]->\n", type);
		const char * error_str_begin = error_str->Ptr;
		const char* cap_ptr;
		if (Ptr - CAPTUREBACK >= Buf) {
			error_str->append("...");
			cap_ptr = Ptr - CAPTUREBACK;
		}
		else {
			cap_ptr = Buf;
		}
		const char* cap_end_ptr = cap_ptr + CAPTURELEN;
		int sign_to_sign_begin = 0;
		int sign_to_sign_end = 0;

		while (cap_ptr != cap_end_ptr) {
			if ((*cap_ptr == ' ') || (*cap_ptr == '\r') ||
				(*cap_ptr == '\t') || (*cap_ptr == '\n')) {
				error_str->append(' ');
			}
			else if (*cap_ptr == 0) {
				break;
			}
			else {
				error_str->append(*cap_ptr);
			}
			cap_ptr++;
			if (sign_begin == cap_ptr) {
				sign_to_sign_begin = error_str->Ptr - error_str_begin;
			}
			if (sign_end == cap_ptr) {
				sign_to_sign_end = error_str->Ptr - error_str_begin;
			}
		}
		if (*cap_ptr != 0) {
			error_str->append("...");
		}
		error_str->append('\n');
		int i = 0;
		for (;i < sign_to_sign_begin;i++) {
			error_str->append(' ');
		}

		for (;i < sign_to_sign_end;i++) {
			error_str->append('^');
		}
		return error_str;
	}
	LString * JsonParseString::followError() {
		append("\t->");
		return error_str;
	}
	LString * JsonParseString::breakError() {
		append("\t->");
		return error_str;
	}


	//关于json数组和类的括号解析
	bool JsonParseString::isBgnHasNext(bool is_array_not_object) {
		char bgn_ch, end_ch;
		if (is_array_not_object) {
			bgn_ch = '[';
			end_ch = ']';
		}
		else {
			bgn_ch = '{';
			end_ch = '}';
		}
		if (checkCh(bgn_ch)) {
			if (checkCh(end_ch)) {
				return false;
			}
			return true;
		}
		else if (checkStr("null")) {
			return false;
		}
		else {
			_last_alram = eAlram::no_begin_token;
			captureAndPrintError("Parse array need '%c' or \"null\"", bgn_ch);
			return false;
		}
	}
	bool JsonParseString::isGapHasNext(bool is_array_not_object) {
		char bgn_ch, end_ch;
		if (is_array_not_object) {
			bgn_ch = '[';
			end_ch = ']';
		}
		else {
			bgn_ch = '{';
			end_ch = '}';
		}
		if (checkCh(',')) {
			return true;
		}
		else if (checkCh(end_ch)) {
			return false;
		}
		else {
			_last_alram = eAlram::no_gap_or_end_token;
			captureAndPrintError("need ',' or '%c'to stop Array", end_ch);
			return false;
		}
	}


	int JsonParseString::parseString(char * str, int size) {
		recordBeginPtr();
		bool is_overflow = (size == 0);
		char * str_end = str + size;
		if (checkCh('"') == false) {
			captureAndPrintError("String should start with '\"'.\n");
			_last_alram = eAlram::not_str;
			return fail;
		}
		while (1) {
			if (*_ptr == '\\') {
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
				if ((is_overflow) && (size != 0)) {
					captureAndPrintError("String buffer is too small for string.\n");
					_last_alram = eAlram::str_overflow;
					return done;
				}
				else {
					_last_alram = eAlram::no_error;
					return done;
				}
			}
			else if ((*_ptr < 0x20) || (*_ptr >= 0x7f)) {
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
				_last_alram = eAlram::no_error;
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
				_last_alram = eAlram::no_error;
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
		_last_alram = eAlram::no_error;
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
		_last_alram = eAlram::no_error;
		*value = temp;
		return done;
	}

};