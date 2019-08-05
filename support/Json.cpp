
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <stdarg.h>
#include "lee.h"

#include "Json.h"
using namespace std;
using namespace srb;
namespace atjson {
	int StringPtr::append(char* append) {
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
	int StringPtr::append(char append) {
		if(ptr_i < size-1){
			buf[ptr_i++] = append;
			buf[ptr_i] = 0;
			return done;
		}
		return fail;
	}
	int StringPtr::printf(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int print_inc = vsnprintf(ptr(), rem(), format, args);
		va_end(args);
		return inc(print_inc);
	}



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

	void json::copyValueFrom(const json& from) {
		size = from.size;
		tran = from.tran;
		name = from.name;
		hash = from.hash;

	}
	void json::moveFrom(json &from){
		copyValueFrom(from);
		value_prt = from.value_prt;
		from.tran = writeNull;
		from.size = 0;
		from.value_prt = nullptr;
	}
	void json::copyFrom(const json &from) {
		copyValueFrom(from);
		if (size != 0) {
			json* new_table = new json[size]();
			json* old_table = (json *)from.value_prt;
			for (int i = 0;i < size;i++) {
				new_table[i].copyFrom(old_table[i]);
			}
			value_prt = new_table;
		}
		else{
			value_prt = from.value_prt;
		}
	}
	json::~json()
	{
		if (size != 0) {
			delete[]((json*)value_prt);
		}
	}

	json json::operator= (const json & right)	{
		copyFrom(right);
		return right;
	}
	json::json(const json& old) {
		copyFrom(old);
	}
	json::json(json&& old) {
		moveFrom(old);
	}




	int writeNull(StringPtr* str, void* value_prt, int size) {
		return str->printf("null");
	}
	json::json() :tran(writeNull) {}

	int writeInt(StringPtr* str, void* value_prt, int size) {
		return str->printf("%d", *((int*)value_prt));
	}

	json::json(int* vp):tran(writeInt),value_prt(vp){}

	json::json(initializer_list<json> v) : tran(writeArray) {
		size = v.size();
		if (size != 0) {
			json* table = new json[size]();
			int i = 0;
			for each (auto var in v) {
				table[i++].moveFrom(var);
			}
			value_prt = table;
		}
	}
	json::json(initializer_list<pair<const char*, json>> v) : tran(writeObject) {
		size = v.size();
		if (size != 0) {
			json* table = new json[size]();
			int i = 0;
			for each (auto var in v) {
				table[i].moveFrom(var.second);
				table[i].name = var.first;
				table[i].hash = getHashString(name);
				i++;
			}
			value_prt = table;
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
		return tran(str, value_prt, size);
	}



	int writeArray(StringPtr* str, void* value_prt,int size)	{
		auto table = (json*)value_prt;
		int i = 0;
		checkFailReturn(str->append('['));
		while (1) {
			if (i < size) {
				checkFailReturn(table[i++].get(str));
			}
			if (i < size) {
				checkFailReturn(str->append(','));
			}
			else {
				checkFailReturn(str->append(']'));
				return done;
			}
		}
	}

	int writeObject(StringPtr* str, void* value_prt, int size) 	{
		auto table = (json*)value_prt;
		int i = 0;
		checkFailReturn(str->append("{"));
		while (1) {
			if (i < size) {
				checkFailReturn(table[i].getJsonKey(str));
				checkFailReturn(table[i].get(str));
				i++;
			}
			if (i < size) {
				checkFailReturn(str->append(','));
			}
			else {
				checkFailReturn(str->append('}'));
				return done;
			}
		}
	}

};