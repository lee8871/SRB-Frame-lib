#pragma once
#include "lee.h"
#include "LString.h"


namespace lee8871_support {
#define __ENUM_lv3__(situation,num)int situation = (-num-__BASE_NUM__);	const char * __const_char_ptr##num = #situation;
#define __ENUM_lv2__(situation,num) __ENUM_lv3__(situation,num)
#define __ENUM(situation)  __ENUM_lv2__(situation,__LINE__)
	constexpr int __BASE_NUM__ = 200- (__LINE__ + 2);
	constexpr struct {
		__ENUM(no_error);
		__ENUM(no_a_num);
		__ENUM(float_to_int);
		__ENUM(float_to_int_overflow);
		__ENUM(negative_to_unsigned);

		__ENUM(overflow);
		__ENUM(get_no_init);
		__ENUM(set_bad_json_str);
		__ENUM(get_not_string);
		__ENUM(get_cc_unpaired_DQM);
	}eJsonParse;
#undef __ENUM
#undef __ENUM_lv2__
#undef __ENUM_lv3__
	inline const char * enumRevJsonGetString(int num) {
		struct sIntCharPtr { int value; const char * p_name; };
		sIntCharPtr * _ptr = (sIntCharPtr *)(&eJsonParse);
		for (int i = 0;i < sizeof(eJsonParse) / sizeof(sIntCharPtr);i++) {
			if (_ptr[i].value == num) {
				return _ptr[i].p_name;
			}
		}
		return "bad_enum_value";
	}	

	class LError {
		LString str;
		int err_num;
		int postError(char* poster, int erro);
	};



	class JsonParseString : public LString {
	protected:
		int tab_level = 0;
		constexpr static int ERROR_CP_SIZE = 4096;
		LString* error_str = nullptr;
		int error_code = eJsonParse.no_error;
	public:
		LString* const& Error_str = error_str;
		bool isExpanded = true;
		char* tab_string = "  ";
		JsonParseString(char *buf, int size);
		JsonParseString(int size);
		~JsonParseString();
		inline void reset() {
			LString::reset();
			tab_level = 0;	
		}

		int parseNumber(int* value);
		int parseNumber(unsigned int * value);
		int parseNumber(float* value);
		int parseNumber(double* value);

		int parseString(char * s, int size);



		int errorOccur(const char * type,int num);
		int errorForwared();
		int errorResolved();

		LString* get_errorString();
		LString*  captureError(const char * type) {
			if (error_str == nullptr) {
				error_str = new LString(ERROR_CP_SIZE);
			}
			char* begin_ptr = error_str->Ptr;
			error_str->print("<%s>-->", type);
			char* cap_ptr;
			if (Ptr - 20 >= Buf) {
				error_str->append("...");
				cap_ptr = Ptr - 20;
			}
			else {
				cap_ptr = Buf;
			}
			int error_diff = 0;
			while (cap_ptr != cap_ptr+40) {
				if ((*cap_ptr == ' ') || (*cap_ptr == '\r') ||
					(*cap_ptr == '\t') || (*cap_ptr == '\n')) {
					error_str->append(' ');
				}
				else if(*cap_ptr == 0){
					break;
				}
				else {
					error_str->append(*cap_ptr);
				}
				if (Ptr == ++cap_ptr) {
					error_diff = error_str->Ptr - begin_ptr;
				}
			}
			error_str->append('\n');
			for (int i = 0;i < error_diff;i++) {
				error_str->append(' ');
			}
			error_str->append("^\n");
			return error_str;
		}

		int bypass() {
			if (checkCh('"')) {

			}
			else if (checkCh('{')) {

			}
			else if (checkCh('[')) {

			}
			else if ((*Ptr >='0')&&(*Ptr <='9')) {

			}
			else {
				captureError("JBP")->print("Json read an unknow char\n");
				return fail;
			}
		}
		bool isBgnHasNext(bool is_array_not_object) {
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
				captureError("J2A")->append("Parse array need '%c' or \"null\"",bgn_ch);
				return false;
			}
		}
		bool isGapHasNext(bool is_array_not_object) {
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
				captureError("J2A")->print("need ',' or '%c'to stop Array", end_ch);
				return false;
			}
		}
	};
};