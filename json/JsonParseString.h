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
		bool isBgnHasNext(bool is_array_not_object);
		bool isGapHasNext(bool is_array_not_object);
	};
};