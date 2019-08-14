#pragma once
#include "lee.h"
#include "JsonParseString.h"
#include "JsonGenerateString.h"
#include <initializer_list>
#include <utility>


namespace lee8871_support {

	class json;

	class JsonTransformer {
	protected:
	public:
		bool isCommon = true;
		virtual int get(JsonGenerateString* str, void *value) = 0;
		virtual int set(JsonParseString* str, void *value) = 0;
		virtual JsonTransformer* copy() = 0;
		virtual~JsonTransformer() {}
	};

	class json:public JsonTransformer {
	private:

	public:
		void * value_prt = nullptr;
		JsonTransformer* transform = nullptr;


		void moveFrom(json&);
		void copyFrom(const json&);
		JsonTransformer* copy() {
			return new json(*this);
		}
		json operator = (const json&);
		json(json&&);
		json(const json&);

		json();

		json(JsonTransformer* transform, void* value_prt);
		json(int32* value_prt);
		json(uint16* value_prt);
		json(uint8* value_prt);
		json(int8 * value_prt);

		json(std::initializer_list<json> v);//to json array
		json(std::initializer_list<std::pair<const char*, json>> v);//to json object

		~json();
		int get(JsonGenerateString* str, void *diff = 0)override;
		int set(JsonParseString* str, void *diff = 0)override;
	};
	   	 
	json buildJsonConstStr(const char * value_prt);
	json buildJsonStr(char * value_prt, int max_size);


	class JSON_INITIELAZATION_CLASS {
	public:
		JSON_INITIELAZATION_CLASS();
	};
	#ifndef NOT_JSON_USER
	static JSON_INITIELAZATION_CLASS __JSON_INITIELAZATION_OBJECT__;
	#endif
};