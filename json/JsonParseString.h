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
		JsonParseString(const char * path);





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
		void recordBeginPtr();

		constexpr static int CAPTURELEN = 80;
		constexpr static int CAPTUREBACK = 30;

		LString* get_errorString();
		LString* captureError(const char * type);
		LString* followError();
		LString* breakError();
		
		int bypassString() {
			while (*Ptr != '"') {
				if (*Ptr == '\\') {
					forward();
					if ((*Ptr < 0x20) || (*Ptr >= 0x7f)) {
						captureAndPrintError("Unknow control char 0x%x\n", *Ptr);
						_last_alram = eAlram::str_token_unpair;
						return fail;
					}
				}
				forward();
				if ((*Ptr < 0x20) || (*Ptr >= 0x7f)) {
					captureAndPrintError("Unknow control char 0x%x\n", *Ptr);
					_last_alram = eAlram::str_token_unpair;
					return fail;
				}
			}
			forward();
			return done;
		}
		int bypass() {
			recordBeginPtr();
			if (*Ptr == '"') {
				bypassString();
				return done;
			}
			else if (*Ptr == '{') {
				if (isBgnHasNext(false)) {
					do {
						if (!checkCh('"')) {
							captureAndPrintError("Object need a string name.");
							return fail;
						}
						checkFailReturn(bypassString());
						if (!checkCh(':')) {
							captureAndPrintError("Object need a ':'");
							return fail;
						}
						checkFailReturn (bypass());
					} while (isGapHasNext(false));
				}
				return done;
			}
			else if (*Ptr =='[') {
				if (isBgnHasNext(true)) {
					do {
						checkFailReturn(bypass());
					} while (isGapHasNext(true));
				}
			}
			else if ((*Ptr >='0')&&(*Ptr <='9')) {
				bypassNum();
			}
			else if (*Ptr == 't') {

			}
			else if (*Ptr == 'f') {

			}
			else if (*Ptr == 'n') {

			}
			else {
				captureAndPrintError("Json read an unknow char\n");
				return fail;
			}
		}
		int bypassNum() {
			if (*Ptr == '-') {
				forward();
			}
			if ((*Ptr == '0')) {
				forward();
			}
			else {
				while ((*Ptr >= '0') && (*Ptr <= '9')) {
					forward();
				}
			}
			if (*Ptr == '.') {
				forward();
			}
			while ((*Ptr >= '0') && (*Ptr <= '9')) {
				forward();
			}
			if (*Ptr == 'e') {
				forward();
			}
			if ((*Ptr == '+')||(*Ptr == '-')) {
				forward();
			}
			while ((*Ptr >= '0') && (*Ptr <= '9')) {
				forward();
			}
		}





		bool isBgnHasNext(bool is_array_not_object);
		bool isGapHasNext(bool is_array_not_object);
	};
};