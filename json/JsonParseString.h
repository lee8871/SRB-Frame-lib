#pragma once
#include "lee.h"
#include "LString.h"


namespace lee8871_support {


#define captureAndPrintError captureError(__FUNCTION__)->print

	class JsonParseString : public LString {
	public:
		enum class eAlram {
			no_error,
			no_a_num,
			float_to_int,
			float_to_int_overflow,
			negative_to_unsigned,
			overflow,

			no_begin_token,
			no_gap_or_end_token,

			not_str,
			str_overflow,
			str_token_unpair
		};
	protected:
		eAlram _last_alram = eAlram::no_error;
		int tab_level = 0;
		constexpr static int ERROR_CP_SIZE = 4096;
		LString* error_str = nullptr;
	public:
		eAlram  const& Last_alram = _last_alram;
		LString* const& Error_str = error_str;
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


		const char* sign_begin = 0;
		void recordBeginPtr() {
			sign_begin = Ptr;
		}

		constexpr static int CAPTURELEN = 80;
		constexpr static int CAPTUREBACK = 30;

		LString* get_errorString();
		LString* captureError(const char * type) {
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
				else if(*cap_ptr == 0){
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
		LString* followError() {
			append("\t->");
			return error_str;
		}
		LString* breakError() {
			append("\t->");
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
				captureAndPrintError("Json read an unknow char\n");
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
				_last_alram = eAlram::no_begin_token;
				captureAndPrintError("Parse array need '%c' or \"null\"",bgn_ch);
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
				_last_alram = eAlram::no_gap_or_end_token;
				captureAndPrintError("need ',' or '%c'to stop Array", end_ch);
				return false;
			}
		}
	};
};