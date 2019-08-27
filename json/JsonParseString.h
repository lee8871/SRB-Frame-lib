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
		void recordBeginPtr();

		constexpr static int CAPTURELEN = 80;
		constexpr static int CAPTUREBACK = 30;

		LString* get_errorString();
		LString* captureError(const char * type);
		LString* followError();
		LString* breakError();
		
		int bypassString() {
			while (*_ptr != '"') {
				if (*_ptr == '\\') {
					_ptr++;
					if ((*_ptr < 0x20) || (*_ptr >= 0x7f)) {
						captureAndPrintError("Unknow control char 0x%x\n", *_ptr);
						_last_alram = eAlram::str_token_unpair;
						return fail;
					}
				}
				_ptr++;
				if ((*_ptr < 0x20) || (*_ptr >= 0x7f)) {
					captureAndPrintError("Unknow control char 0x%x\n", *_ptr);
					_last_alram = eAlram::str_token_unpair;
					return fail;
				}
			}
			_ptr++;
			return done;
		}
		int bypass() {
			recordBeginPtr();
			if (*_ptr == '"') {
				bypassString();
				return done;
			}
			else if (*_ptr == '{') {
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
			else if (*_ptr =='[') {
				if (isBgnHasNext(true)) {
					do {
						checkFailReturn(bypass());
					} while (isGapHasNext(true));
				}
			}
			else if ((*_ptr >='0')&&(*_ptr <='9')) {
				bypassNum();
			}
			else if (*_ptr == 't') {

			}
			else if (*_ptr == 'f') {

			}
			else if (*_ptr == 'n') {

			}
			else {
				captureAndPrintError("Json read an unknow char\n");
				return fail;
			}
		}
		int bypassNum() {
			if (*_ptr == '-') {
				_ptr++;
			}
			if ((*_ptr == '0')) {
				_ptr++;
			}
			else {
				while ((*_ptr >= '0') && (*_ptr <= '9')) {
					_ptr++;
				}
			}
			if (*_ptr == '.') {
				_ptr++;
			}
			while ((*_ptr >= '0') && (*_ptr <= '9')) {
				_ptr++;
			}
			if (*_ptr == 'e') {
				_ptr++;
			}
			if ((*_ptr == '+')||(*_ptr == '-')) {
				_ptr++;
			}
			while ((*_ptr >= '0') && (*_ptr <= '9')) {
				_ptr++;
			}


		}





		bool isBgnHasNext(bool is_array_not_object);
		bool isGapHasNext(bool is_array_not_object);
	};
};