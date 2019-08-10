#include "Json.h"

using namespace std;
namespace lee8871_support {



	json::json(transformCB transform, void * value_prt) : transform(transform), value_prt(value_prt) {}


#include "transform_define.in.h"
	int asError(transformCBArgumenrt) {
		str->get_errorString()->print(
			"[LibError], No initialized json is %s, it is a bug in [jsonlib].", 
			(is_get ? "serialized" : "deserialized"));
		return eRevJson.get_no_init;
	}
	json::json() :transform(asError) {}
	int asInt(transformCBArgumenrt) {
		if(is_get){
			int temp = *valuePtr(int);
			return str->inputNumber(temp);
		}
		else {
			int rev = str->outputNumber(valuePtr(int));
			switch(rev){
			case done:
				return done;
			case eRevJson.get_no_num:
				str->reportError("[ErrorDse]")->print("get number fail\n");
				return eRevJson.get_no_num;
			case eRevJson.get_float_to_int:
				str->reportError("[WarningDse]")->print("get int from float\n");
				return done;
			case eRevJson.overflow:
				str->reportError("[WarningDse]")->print("int num may big than int_max\n");
				return done;
			default:
				str->reportError("[ErrorLIB]")->print("Unexpected case %d",rev);
			}
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
		if (is_get) {
			return str->inputString(valuePtr(char));
		}
		else {
			int rev = str->outputString(valuePtr(char),obj->size);
			switch (rev) {
			}
		}
	}


	json jsonString(const char * value_prt) {
		json rev{ asCharString, (void*)value_prt };
		return rev;
	}






};