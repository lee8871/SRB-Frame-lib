
#include <stdio.h>
#include <typeinfo>
#include <string>
#include "lee.h"

#include "Json.h"
using namespace std;
using namespace srb;
namespace atjson {
	enum class eJsonType {
		error,
		i8,
		i16,
		i32,
		u8,
		u16,
		u32,
		obj
	};
	class StringPtr {
	private:
		char* bgn;
		size_t ptr_i = 0;
		size_t size;
	public:
		StringPtr(char* buf, int size) :
			bgn(buf), size(size) {}

		inline char* ptr() {
			return bgn + ptr_i;
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
		inline int rem() {
			return (int)(size - ptr_i);
		}
	};
	constexpr unsigned int HASH_ERROR = 0;
	static unsigned int getHashString(const char * str) {
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
		return HASH_ERROR;
	}
	static unsigned int getHashByDoubleQuotes(const char * str) {
		int i = 0;
		unsigned int id = 0;
		unsigned int id_temp = 1;
		const int mod = 1610612741;
		unsigned char mapped = 0;
		if (str[i] != '"') {
			return HASH_ERROR;
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
		return HASH_ERROR;
	}

	json::json(const char* name, int* value_prt, int len)
		:value_prt(value_prt), len(len), name(name),
		type(eJsonType::i32) {

		hash = getHashString(name);
		if (hash == HASH_ERROR){
			type = eJsonType::error;
		}
	}
	json::json(const char* name, json* value_prt, int len)
		: value_prt(value_prt), len(len), name(name),
		type(eJsonType::obj) {

		hash = getHashString(name);
		if (hash == HASH_ERROR) {
			type = eJsonType::error;
		}
	}

#define writeToBuf(format, ...) do{\
	if (done != str->inc(snprintf(str->ptr(), str->rem(), format, __VA_ARGS__)))return -1;	\
	}while(0)
	
	int getJson(char * buf, int buf_size, json* object, int len) {
		StringPtr str_{ buf,buf_size };
		StringPtr * str = &str_;
		writeToBuf("{");
		int i = 0;
		if (object[i++].get(str) < 0)return fail;
		while (i < len) {
			writeToBuf(",");
			if (object[i++].get(str) < 0)return fail;
		}
		writeToBuf("}");
		return done;
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

	int json::get(StringPtr* str) {
		switch (type) {
		case eJsonType::obj:
			return get_obj(str);
		case eJsonType::i32:
			return get_i32(str);
		}
		return -1;
	}

	int json::get_obj(StringPtr* str) {
		json* objs = (json*)value_prt;
		writeToBuf("\"%s\":", name);
		if (len == 0) {
			writeToBuf("null");
		}
		else {
			int i = 0;
			writeToBuf("{");
			if (objs[i++].get(str) < 0)return fail;
			while (i < len) {
				writeToBuf(",");
				if (objs[i++].get(str) < 0)return fail;
			}
			writeToBuf("}");
		}
		return done;
	}
	int json::get_i32(StringPtr* str) {
		int* val = (int*)value_prt;
		writeToBuf("\"%s\":", name);
		switch (len) {
		case 0:
			writeToBuf("null");
			break;
		case 1:
			writeToBuf("%d", val[0]);
			break;
		default:
			int i = 0;
			writeToBuf("[%d", val[i++]);
			while (i < len) {
				writeToBuf(",%d", val[i++]);
			}
			writeToBuf("]");
			break;
		}
		return done;
	}
};