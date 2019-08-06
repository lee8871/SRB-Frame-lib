#include "Json.h"

using namespace std;
namespace lee8871_support {

	int asError(JsonString* str, void* value_prt, bool is_get) {
		return str->printf("error");
	}
	json::json() :transform(asError) {}

	int asInt(JsonString* str, void* value_prt, bool is_get) {
		int temp = *((int*)value_prt);
		return str->inputNumber(temp);
	}

	int asUint16(JsonString* str, void* value_prt, bool is_get) {
		unsigned int temp = *((uint16*)value_prt);
		return str->inputNumber(temp);
	}

	int asUint8(JsonString* str, void* value_prt, bool is_get) {
		unsigned int temp = *((uint8*)value_prt);
		return str->inputNumber(temp);
	}

	int asInt8(JsonString* str, void* value_prt, bool is_get) {
		int temp = *((int8*)value_prt);
		return str->inputNumber(temp);
	}





	json::json(int32* vp) : transform(asInt), value_prt(vp) {}
	json::json(uint16 * value_prt) : transform(asUint16), value_prt(value_prt) {}
	json::json(uint8 * value_prt) : transform(asUint8), value_prt(value_prt) {}
	json::json(int8 * value_prt) : transform(asInt8), value_prt((void*)value_prt) {}












	int asCharString(JsonString* str, void* value_prt, bool is_get) {
		return str->inputString((char *)value_prt);
	}
	json jsonString(const char * value_prt) {
		json rev{ asCharString, (void*)value_prt };
		return rev;
	}





};