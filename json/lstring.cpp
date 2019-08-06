#include "lstring.h"
#include <stdarg.h>
#include <stdio.h>


namespace lee8871_support {
	int LString::append(char* append) {
		while (1) {
			if (*append == 0) {
				buf[ptr_i] =0;
				return done;
			}
			else{
				buf[ptr_i++] = *append;
				if (ptr_i < size) {
					append++;
				}
				else {
					buf[size - 1] = 0;
					return fail;
				}
			}
		}
	}
	int LString::append(char append) {
		if(ptr_i < size-1){
			buf[ptr_i++] = append;
			buf[ptr_i] = 0;
			return done;
		}
		return fail;
	}
	int LString::printf(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int print_inc = vsnprintf(ptr(), rem(), format, args);
		va_end(args);
		return inc(print_inc);
	}


	const int HASH_MOD = 1610612741;
	unsigned int getHashString(const char * str) {
		if (str == nullptr) {
			return NO_HASH;
		}
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int HASH_MOD = 1610612741;
		unsigned char mapped = 0;
		while (i < 255) {
			if (str[i] == 0) { return (id^id_temp); }
			id_temp ^= ((unsigned int)str[i]) << (1 + (i % 4) * 7);
			i++;
			if ((i % 4) == 0) {
				id ^= id_temp;
				id *= HASH_MOD;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}
	unsigned int getHashByDoubleQuotes(const char * str) {
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int HASH_MOD = 1610612741;
		unsigned char mapped = 0;
		if (str[i] != '"') {
			return NO_HASH;
		}
		i++;
		while (i<255) {
			if (str[i] == '"') { return (id^id_temp); }
			id_temp ^= ((unsigned int)str[i]) << (1 + (i % 4) * 7);
			i++;
			if ((i % 4) == 0) {
				id ^= id_temp;
				id *= HASH_MOD;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}
};