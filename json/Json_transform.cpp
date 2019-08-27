#define NOT_JSON_USER
#include "Json.in.h"

using namespace std;
using namespace srb;
namespace lee8871_support {
	class asUint16 : public iJsonTransformer{
	public:
		int get(JsonGenerateString* str, const void* value)override {
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
	class asUint8 : public iJsonTransformer{
	public:
		int get(JsonGenerateString* str, const void* value)override {
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

	class asInt8 : public iJsonTransformer {
	public:
		int get(JsonGenerateString* str, const void* value)override {
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

	class asInt : public iJsonTransformer {
	public:
		int get(JsonGenerateString* str, const void* value)override {
			int temp = *(int*)value;
			str->inputNumber(temp);
			return str->checkOverflow();
		};
		int set(JsonParseString* str, void *value)override {
			return str->parseNumber((int*)value);
		};
	};






	static int is_init_counter = 0;
	static class asInt* casI32 = nullptr;
	static class asUint16* casU16 = nullptr;
	static class asUint8* casU8 = nullptr;
	static class asInt8* casI8 = nullptr;
	JSON_INITIELAZATION_CLASS::JSON_INITIELAZATION_CLASS()	{

		if (is_init_counter == 0) {

			JsonLog = new ModuleLog("json",	eLogLevel::info ,
				__DATE__, __TIME__);
			casU16 = new asUint16;
			casU8 = new asUint8;
			casI8 = new asInt8;

			casI32 = new asInt;
		}
		is_init_counter++;
	}

	JSON_INITIELAZATION_CLASS::~JSON_INITIELAZATION_CLASS(){
		is_init_counter--;
		if (is_init_counter == 0) {

			delete casU16;
			delete casU8;
			delete casI8;
			delete casI32;
			delete JsonLog;

		}
	}

	Json::Json(int32* value_prt) : _transform(casI32->quote()), value_prt(value_prt) {}
	Json::Json(uint16 * value_prt) : _transform(casU16->quote()), value_prt(value_prt) {}
	Json::Json(uint8 * value_prt) : _transform(casU8->quote()), value_prt(value_prt) {}
	Json::Json(int8 * value_prt) : _transform(casI8->quote()), value_prt(value_prt) {}




	class asConstStr : public iJsonTransformer {
	private:
		const char * _const_str;
	public:
		asConstStr(const char * const_str) {
			_const_str = const_str;
			INFO("new asConstStr(%s)", _const_str);
		}
		~asConstStr()		{
			INFO("delete asConstStr(%s)", _const_str);
		}
		int get(JsonGenerateString* str, const void* value)override {
			str->inputString(_const_str);
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			return str->parseString(0, 0);
		};
	};

	Json buildJsonConstStr(const char * value_prt) {
		asConstStr*  ts = new asConstStr(value_prt);
		Json rev{ ts, nullptr };
		ts->freeQuote();
		return rev;
	}

	class asStr : public iJsonTransformer {
	private:
		int _str_size;
	public:
		asStr(int size) {
			_str_size = size;
			INFO("new asStr(%d)", _str_size);
		}
		~asStr() {
			INFO("delete asStr(%d)", _str_size);
		}
		int get(JsonGenerateString* str, const void* value)override {
			str->inputString((char*)value);
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			return str->parseString((char*)value, _str_size);
		};
	};

	Json buildJsonStr(char * value_prt,int max_size) {
		auto  transformer = new asStr(max_size);
		Json rev{ transformer, value_prt };
		transformer->freeQuote();
		return rev;
	}

	class asUint8Str : public iJsonTransformer {
	private:
		int _str_size;
		char lowToChar(uint8  u8) {
			char c = '0' + (u8 & 0x0f);
			if (c > '9') { c = c - '9' - 1 + 'A'; }
			return c;
		}
		uint8 charToU8(char ch) {
			uint8 u8 = ch - '0';
			if (u8 > 9) {
				u8 = ch - 'A' + 10;
				if (u8 > 15) {
					u8 = ch - 'a' + 10;
					if (u8 > 15) {
						WARNING("Hex string to unit8 bad char %c", ch);
						return 0;
					}
				}
			}
		}
		asUint8Str(int size) {
			_str_size = size;
		}
		~asUint8Str() {
		}


	public:
		int get(JsonGenerateString* str, const void* value)override {
			str->append('"');
			for (int i = 0;i < _str_size;i++) {
				uint8 a = ((uint8*)value)[i];
				str->append(lowToChar(a >> 4));
				str->append(lowToChar(a));
				str->append(' ');
			}
			str->append('"');
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			ERROR("Add set methord");
			return 0;
		};
		friend Json buildUint8Str(uint8 * value_prt, int max_size);
	};

	Json buildUint8Str(uint8 * value_prt, int max_size) {
		auto transformer = new asUint8Str(max_size);
		Json rev{ transformer, value_prt };
		transformer->freeQuote();
		return rev;
	}


	class asUint8Array : public iJsonTransformer {
	private:
		int _str_size;
		size_t size_each = 1;
		asUint8Array(int size) {
			_str_size = size;
		}
		~asUint8Array() {
		}
	public:
		int get(JsonGenerateString* str, const void* value)override {
			str->arrayBgn();
			int i = 0;
			while(1) {
				casU8->get(str, (void*)((size_t)value + i * size_each));
				i++;
				if (i < _str_size) {
					str->arrayGap();
				}
				else{
					str->arrayEnd();
					break;
				}
			}
			return str->checkOverflow();
		}
		int set(JsonParseString* str, void * value)override {
			ERROR("Add set methord");
			return 0;
		};
		friend Json buildUint8Array(uint8 * value_prt, int max_size);
	};

	Json buildUint8Array(uint8 * value_prt, int max_size) {
		auto transformer = new asUint8Array(max_size);
		Json rev{ transformer, value_prt };
		transformer->freeQuote();
		return rev;
	}
	
	class JsonPtr :public iJsonTransformer {
	private:
		void * value_prt = nullptr;
		iJsonTransformer* _transform = nullptr;
		JsonPtr(const Json& json) {
			value_prt = json.value_prt;
			_transform = json._transform;
			_transform->quote();
		}
		JsonPtr(const JsonPtr& json) = delete;
		JsonPtr(JsonPtr&& json) = delete;
		~JsonPtr() {
			if (_transform != nullptr) {
				_transform->freeQuote();
			}
		}
	public:
		int get(JsonGenerateString* str, const void* diff)override {
			return _transform->get(str, *(void**)((size_t)diff + (size_t)value_prt));
		};
		int set(JsonParseString* str, void *diff)override {
			return _transform->set(str, *(void**)((size_t)diff + (size_t)value_prt));
		};
		friend Json buildJsonPtr(const Json& json, void* Value_pp);
	};
	Json buildJsonPtr(const Json& json, void* Value_pp) {
		auto jp = new JsonPtr(json);
		Json rev{ jp, Value_pp };
		jp->freeQuote();
		return rev;
	}






};
