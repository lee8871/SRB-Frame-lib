
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <stdarg.h>
#include "lee.h"
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
		from.transform = asNull;
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







	int asNull(LString* str, void* value_prt, bool is_get) {
		return str->printf("null");
	}
	int asArray(LString* str, void* value_prt, bool is_get) {
		return str->printf("[]");
	}
	int asObject(LString* str, void* value_prt, bool is_get) {
		return str->printf("{}");
	}
	json::json() :transform(asNull) {}

	int asInt(LString* str, void* value_prt,bool is_get) {
		return str->printf("%d", *((int*)value_prt));
	}
	json::json(int32* vp):transform(asInt),value_prt(vp){}


	int asUint16(LString* str, void* value_prt, bool is_get) {
		return str->printf("%u", *((uint16*)value_prt));
	}
	json::json(uint16 * value_prt):transform(asUint16), value_prt(value_prt) {}


	int asUint8(LString* str, void* value_prt, bool is_get) {
		return str->printf("%u", *((uint8*)value_prt));
	}
	json::json(uint8 * value_prt):transform(asUint8), value_prt(value_prt) {}

	int asInt8(LString* str, void* value_prt, bool is_get) {
		return str->printf("%d", *((int8 *)value_prt));
	}
	json::json(int8 * value_prt) : transform(asInt8), value_prt((void*)value_prt) {}

	int asCharString(LString* str, void* value_prt, bool is_get) {
		return str->printf("\"%s\"", ((char *)value_prt));
	}
	json jsonString(const char * value_prt) {
		json rev{ asCharString, (void*)value_prt };
		return rev;
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





	int json::getJsonKey(LString* str) {
		return str->printf("\"%s\":", name);
	}

	int json::get(LString* str, void *diff) {
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


	int json::getArray(LString* str, void *diff)	{
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

	int json::getObject(LString* str, void *diff) 	{
		auto table = (json*)value_prt;
		int i = 0;
		checkFailReturn(str->append("{"));
		while (1) {
			if (i < size) {
				checkFailReturn(table[i].getJsonKey(str));
				checkFailReturn(table[i++].get(str, diff));
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