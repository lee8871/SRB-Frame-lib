
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <stdarg.h>
#include "lee.h"

#include "Json.h"
using namespace std;
using namespace srb;
namespace atjson {

	class StringPtr {
	private:
		char* buf;
		size_t ptr_i = 0;
		size_t size;
	public:
		StringPtr(char* b, int s) :
			buf(b), size(s) {
			buf[size-1] = 0;
		}

		inline char* ptr() {
			return buf + ptr_i;
		}
		inline int inc(int increase) {
			if (increase < 0) {
				return -100 + increase;
			}
			ptr_i += increase;
			if (ptr_i == size) {
				return fail;
			}
			return done;
		}
		inline size_t rem() {
			return (size - ptr_i);
		}
		int append(char* append) {
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
		int append(char append) {
			if(ptr_i < size-1){
				buf[ptr_i++] = append;
				buf[ptr_i] = 0;
				return done;
			}
			return fail;
		}
		int printf(const char *format, ...) {
			va_list args;
			va_start(args, format);
			int print_inc = vsnprintf(ptr(), rem(), format, args);
			va_end(args);
			return inc(print_inc);
		}

	};
	constexpr unsigned int NO_HASH = 0;
	static unsigned int getHashString(const char * str) {
		if (str == nullptr) {
			return NO_HASH;
		}
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int mod = 1610612741;
		unsigned char mapped = 0;
		while (i < 255) {
			if (str[i] == 0) { return (id^id_temp); }
			id_temp ^= ((unsigned int)str[i]) << (1 + (i % 4) * 7);
			i++;
			if ((i % 4) == 0) {
				id ^= id_temp;
				id *= mod;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}
	static unsigned int getHashByDoubleQuotes(const char * str) {
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int mod = 1610612741;
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
				id *= mod;
				id_temp = 1;
			}
		}
		return NO_HASH;
	}

	enum class eJsonType {
		error,
		i8,
		i16,
		i32,
		u8,
		u16,
		u32,
		obj,
		arr
	};

	json::json(const char * name, int* value_prt)
		:value_prt(value_prt), name(name), type(eJsonType::i32)
		, _test_pointJson(nullptr)
	{
		hash = getHashString(name);
	}

	json::json(int * value_prt)
		: value_prt(value_prt), name(nullptr), type(eJsonType::i32), hash(NO_HASH)
		, _test_pointJson(nullptr) {}

	json::json(const char * name, json * value_prt, int len, bool is_array)

		: value_prt(value_prt), name(name), len(len)
		, _test_pointJson(nullptr)
	{
		hash = getHashString(name);
		if (is_array) {
			type = eJsonType::arr;
		}
		else {
			type = eJsonType::obj;
		}
	}

	json::json(json * value_prt, int len, bool is_array)

		: value_prt(value_prt),  len(len)
		, _test_pointJson(nullptr)
	{
		if (is_array) {
			type = eJsonType::arr;
		}
		else {
			type = eJsonType::obj;
		}
	}

	json::json(const char * n, initializer_list<json> v, bool is_array):
		_test_pointJson(v.begin())
	{
		value_prt = ((void *)(v.begin()));
		name = n;
		len = v.size();
		hash = getHashString(name);
		if (is_array) {
			type = eJsonType::arr;
		}
		else {
			type = eJsonType::obj;
		}
	}



	int checkchar(const char* &buf, char check) {
		if (*buf == check) {
			buf++;
			return 0;
		}
		else {

			return -1;
		}
	}

	int json::set(const char * buf) {
		sscanf(buf, R"("%[^"]":)");
		return 0;
	}





#define checkFailReturn(value) do{\
	int rev = (value);\
	if (done != rev){return rev;}\
}while(0)

	int json::getJson(char * buf, int buf_size) {
		StringPtr str_{ buf,buf_size };
		StringPtr * str = &str_;
		return get(str);
	}

	int json::getJsonKey(StringPtr* str) {
		return str->printf("\"%s\":", name);
	}

	int json::get(StringPtr* str) {
		int i = 0;
		switch (type) {
		case eJsonType::obj:
			i = 0;
			checkFailReturn(str->append("{"));
			while (1) {
				if (i < len) {
					checkFailReturn(((json*)value_prt)[i].getJsonKey(str));
					checkFailReturn(((json*)value_prt)[i].get(str));
					i++;
				}
				if (i < len) {
					checkFailReturn(str->append(','));
				}
				else {
					checkFailReturn(str->append('}'));
					return done;
				}
			}
		case eJsonType::arr:
			checkFailReturn(str->append("["));
			while (1) {
				if (i < len) {
					checkFailReturn(((json*)value_prt)[i].get(str));
					i++;
				}
				if (i < len) {
					checkFailReturn(str->append(','));
				}
				else {
					checkFailReturn(str->append(']'));
					return done;
				}
			}
		default:		
			checkFailReturn(writeValue(str));
			return done ;
		}
	}

	int json::writeValue(StringPtr* str) {
		switch (type) {
		case eJsonType::i32:
			return str->printf("%d", *((int*)value_prt));
		case eJsonType::arr:
			return ((json*)value_prt)->get(str);
		}
	}





};