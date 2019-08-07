#include "Json.h"

using namespace std;
namespace lee8871_support {

#include "transform_define.in.h"
	int asError(transformCBArgumenrt) {
		if (is_get) {
			//LString err_str{ str->errorReport(1024),1024 };
			//err_str.printf("json lib error, no initialized json is serializing");
			return fail;
		}
		else {
			//LString err_str{ str->errorReport(1024),1024 };
			//err_str.printf("json lib error, no initialized json is deserializing");
			return fail;
		}
	}
	json::json() :transform(asError) {}

	int asInt(transformCBArgumenrt) {
		if(is_get){
			int temp = *valuePtr(int);
			return str->inputNumber(temp);
		}
		else {
			return str->outputNumber(valuePtr(int));
		}
	}

	int asUint16(transformCBArgumenrt) {
		unsigned int temp = *valuePtr(uint16);
		return str->inputNumber(temp);
	}

	int asUint8(transformCBArgumenrt) {
		unsigned int temp = *valuePtr(uint8);
		return str->inputNumber(temp);
	}

	int asInt8(transformCBArgumenrt) {
		int temp = *valuePtr(int8);
		return str->inputNumber(temp);
	}




	json::json(int32* vp) : transform(asInt), value_prt(vp) {}
	json::json(uint16 * value_prt) : transform(asUint16), value_prt(value_prt) {}
	json::json(uint8 * value_prt) : transform(asUint8), value_prt(value_prt) {}
	json::json(int8 * value_prt) : transform(asInt8), value_prt((void*)value_prt) {}






	int asCharString(transformCBArgumenrt) {
		return str->inputString(valuePtr(char));
	}
	json jsonString(const char * value_prt) {
		json rev{ asCharString, (void*)value_prt };
		return rev;
	}


	int asArray(transformCBArgumenrt) {
		return obj->getArray(str, diff);
	}
	int asObject(transformCBArgumenrt) {
		return obj->getObject(str, diff);
	}



};