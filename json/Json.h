#pragma once
#include <initializer_list>
#include <utility>

#include "lee.h"
#include "JsonParseString.h"
#include "JsonGenerateString.h"


namespace lee8871_support {
	class JsonTransformer {
	private:
		int quote_counter = 1;
	protected:
		JsonTransformer() {	}
		JsonTransformer(JsonTransformer&) = delete;
		JsonTransformer(JsonTransformer&&) = delete;
		virtual~JsonTransformer() {	}
	public:
		JsonTransformer*  quote() {
			quote_counter++;
			return this;
		}
		void freeQuote() {
			if(--quote_counter == 0){
				delete this;
			}
		}
		virtual int get(JsonGenerateString* str, void *value) = 0;
		virtual int set(JsonParseString* str, void *value) = 0;
	};

	class json:public JsonTransformer {
	private:

	public:
		void * value_prt = nullptr;
		JsonTransformer* transform = nullptr;

		void moveFrom(json&);
		void copyFrom(const json&);
		JsonTransformer* copy() {
			return this;
		}
		json operator = (const json&);
		json(json&&);
		json(const json&);

		json();

		json(JsonTransformer* transform, void* value_prt);
		json(srb::int32* value_prt);
		json(srb::uint16* value_prt);
		json(srb::uint8* value_prt);
		json(srb::int8 * value_prt);

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
		~JSON_INITIELAZATION_CLASS();
	};
	#ifndef NOT_JSON_USER
	static JSON_INITIELAZATION_CLASS __JSON_INITIELAZATION_OBJECT__;
	#endif
};