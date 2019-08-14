#define NOT_JSON_USER
#include "Json.h"

using namespace std;
namespace lee8871_support {
	class asUint16 : public JsonTransformer{
	public:
		JsonTransformer* copy() override {
			return this;
		}
		int get(JsonGenerateString* str, void* value)override {
			int temp = *(int8*)value;
			 str->inputNumber(temp);
			 return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			return fail;
		};
	};

	class asUint8 : public JsonTransformer{
	public:
		JsonTransformer* copy() override {
			return this;
		}
		int get(JsonGenerateString* str, void* value)override {
			int temp = *(int8*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			return fail;
		};
	};
	
	class asInt8 : public JsonTransformer {
	public:
		JsonTransformer* copy() override {
			return this;
		}
		int get(JsonGenerateString* str, void* value)override {
			int temp = *(int8*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			return fail;
		};
	};

	class asInt : public JsonTransformer {
	public:
		JsonTransformer* copy() override {
			return this;
		}
		int get(JsonGenerateString* str, void* value)override {
			int temp = *(int*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			int rev = str->parseNumber((int*)value);
			switch (rev) {
			case done:
				return done;
			case eJsonParse.no_a_num:
				str->get_errorString()->append("->Transform.");
				return eJsonParse.no_a_num;
			case eJsonParse.float_to_int:
				str->get_errorString()->append("->Transform abonden.\n\n");
				return done;
			case eJsonParse.overflow:
				str->get_errorString()->append("->Transform abonden.\n\n");
				return done;
			default:
				str->captureError("[Transform]")->print("Unexpected case %d", rev);
				return fail;
			}
		};
	};
	static bool __is_inited__ = false;
	static class asInt* casI32 = nullptr;
	static class asUint16* casU16 = nullptr;
	static class asUint8* casU8 = nullptr;
	static class asInt8* casI8 = nullptr;

	JSON_INITIELAZATION_CLASS::JSON_INITIELAZATION_CLASS()	{
		if (__is_inited__){return;}
		__is_inited__ = true;
		casU16 = new asUint16;
		casU8 = new asUint8;
		casI8 = new asInt8;
		casI32 = new asInt;
	}
	
	json::json(int32* value_prt) : transform(casI32), value_prt(value_prt) {}
	json::json(uint16 * value_prt) : transform(casU16), value_prt(value_prt) {}
	json::json(uint8 * value_prt) : transform(casU8), value_prt(value_prt) {}
	json::json(int8 * value_prt) : transform(casI8), value_prt(value_prt) {}




	class asConstStr : public JsonTransformer {
	private:
		const char * _const_str;
	public:
		asConstStr(const char * const_str) {
			isCommon = false;
			_const_str = const_str;
		}
		~asConstStr(){}
		JsonTransformer* copy() override {
			return new asConstStr(_const_str);
		}
		int get(JsonGenerateString* str, void* value)override {
			str->inputString(_const_str);
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			return str->parseString(0, 0);
		};
	};



	json buildJsonConstStr(const char * value_prt) {
		json rev{ new asConstStr(value_prt), 0 };
		return rev;
	}

	class asStr : public JsonTransformer {
	private:
		int _str_size;
	public:
		asStr(int size) {
			isCommon = false;
			_str_size = size;
		}
		~asStr() {}
		JsonTransformer* copy() override {
			return new asStr(_str_size);
		}
		int get(JsonGenerateString* str, void* value)override {
			str->inputString((char*)value);
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			return str->parseString((char*)value, _str_size);
		};
	};
	   
	json buildJsonStr(char * value_prt,int max_size) {
		json rev{ new asStr(max_size), value_prt };
		return rev;
	}





};