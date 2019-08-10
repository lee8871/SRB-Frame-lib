#pragma once
#include "lee.h"
#include "JsonString.h"
#include <initializer_list>
#include <utility>
#include "transform_define.in.h"


namespace lee8871_support {

	class json;
	using transformCB = int(*)(transformCBArgumenrt);
	int asError(transformCBArgumenrt);

	class JsonTransformer {
		virtual int get(JsonString* str, void *diff = 0) = 0;
		virtual int set(JsonString* str, void *diff = 0) = 0;
		virtual void*  copy(const void * from) = 0;


	};


	class json{
	private:


		friend int asArray(transformCBArgumenrt);
		friend int asObject(transformCBArgumenrt);


		void copyValueFrom(const json&);
		void moveFrom(json&);
		void copyFrom(const json&);


	public:
		void * value_prt = nullptr;
		int size = 0;
		JsonTransformer* jt = nullptr;

		transformCB transform = asError;

		json operator = (const json&);
		json(json&&);
		json(const json&);

		json();

		json(transformCB transform, void* value_prt);
		json(int32* value_prt);
		json(uint16* value_prt);
		json(uint8* value_prt);
		json(int8 * value_prt);

		json(std::initializer_list<json> v);//to json array
		json(std::initializer_list<std::pair<const char*, json>> v);//to json object

		~json();




		int get(JsonString* str, void *diff = 0);
		int set(JsonString* str, void *diff = 0);
	};
	   	 
	json jsonString(const char * value_prt);


#define __ENUM(situation,num) \
		int situation = num;		const char * __##situation##_name = #situation;	
	constexpr struct {
		__ENUM(get_no_num, -201);
		__ENUM(get_float_to_int, -202);
		__ENUM(get_other_string, -203);
		__ENUM(get_negative_to_unsigned, -204);
		__ENUM(overflow, -205);
		__ENUM(get_no_init, -206);
		__ENUM(set_bad_json_str, -207);
		__ENUM(get_not_string, -208);
	}eRevJson;
#undef __ENUM
	inline const char * enumRevJsonGetString(int num) {
		struct sIntCharPtr { int value; const char * p_name; };
		sIntCharPtr * _ptr = (sIntCharPtr *)(&eRevJson);
		for (int i = 0;i < sizeof(eRevJson) / sizeof(sIntCharPtr);i++) {
			if (_ptr[i].value == num) {
				return _ptr[i].p_name;
			}
		}
		return "bad_enum_value";
	}

};