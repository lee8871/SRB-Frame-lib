#include "Json.h"

using namespace std;
namespace lee8871_support {

	void json::copyValueFrom(const json& from) {
		size = from.size;
		transform = from.transform;
		name = from.name;
		hash = from.hash;

	}
	void json::moveFrom(json &from){
		copyValueFrom(from);
		value_prt = from.value_prt;
		from.transform = asError;
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
	json::~json()	{
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


	int asArray(JsonString* str, void* value_prt, bool is_get) {
		return str->printf("[]");
	}
	int asObject(JsonString* str, void* value_prt, bool is_get) {
		return str->printf("{}");
	}
	   

	json::json(transformCB transform, void * value_prt) : transform(transform), value_prt(value_prt) {}




	json::json(initializer_list<json> v) : transform(asArray) {
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



	constexpr unsigned int NO_HASH = 0;
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
		while (i < 255) {
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


	json::json(initializer_list<pair<const char*, json>> v) : transform(asObject) {
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





		

	int json::get(JsonString* str, void *diff) {
		if (size != 0) {
			if (asArray == transform) {
				return getArray(str, diff);
			}
			else if (asObject == transform) {
				return getObject(str, diff);
			}
			else {
				throw "{{\"__json_lib_error__\":\"size is %d(should be 0),but transform callback unknow\"}}";
				return str->printf(
					"{{\"__json_lib_error__\":\"size is %d(should be 0),but transform callback unknow\"}}", size);
			}
		}
		else{
			return transform(str, (void*)((size_t)value_prt + (size_t)diff),true);
		}
	}


	#define checkFailReturn(value) do{\
		int rev = (value);\
		if (done != rev){return rev;}\
	}while(0)


	int json::getArray(JsonString* str, void *diff)	{
		auto table = (json*)value_prt;
		int i = 0;
		checkFailReturn(str->append('['));
		while (1) {
			if (i < size) {
				checkFailReturn(table[i++].get(str, diff));
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

	int json::getObject(JsonString* str, void *diff) 	{
		auto table = (json*)value_prt;
		int i = 0;
		checkFailReturn(str->objectBgn());
		while (1) {
			if (i < size) {
				checkFailReturn(str->inputString(table[i].name));
				checkFailReturn(str->append(':'));
				checkFailReturn(table[i++].get(str, diff));
			}
			if (i < size) {
				checkFailReturn(str->objectGasket());
			}
			else {
				checkFailReturn(str->objectEnd());
				return done;
			}
		}
	}
};