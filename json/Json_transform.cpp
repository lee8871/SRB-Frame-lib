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
			unsigned int temp = *(uint16*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			unsigned int temp;
			checkFailReturn(str->parseNumber(&temp));
			if (temp > UINT16_MAX) {
				temp = UINT16_MAX;
				str->captureAndPrintError("value > uint16.max\n");
			}
			*(uint16*)value = temp;
			return done;
		};
	};

	class asUint8 : public JsonTransformer{
	public:
		JsonTransformer* copy() override {
			return this;
		}
		int get(JsonGenerateString* str, void* value)override {
			unsigned int temp = *(uint8*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			unsigned int temp;

			checkFailReturn(str->parseNumber(&temp));
			if (temp > UINT8_MAX) {
				temp = UINT8_MAX;
				str->captureAndPrintError("value > uint8.max\n");
			}
			*(uint8*)value = temp;
			return done;
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
			unsigned int temp;
			checkFailReturn(str->parseNumber(&temp));
			if (temp < INT8_MIN) {
				temp = INT8_MIN;
				str->captureAndPrintError("value < int8.min\n");
			}
			if (temp > INT8_MAX) {
				temp = INT8_MAX;
				str->captureAndPrintError("value > int8.max\n");
			}
			*(int8*)value = temp;
			return done;
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
			return str->parseNumber((int*)value);
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