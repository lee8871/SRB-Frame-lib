#pragma once
#include <initializer_list>
#include <utility>
#include "lee.h"
#include "JsonParseString.h"
#include "JsonGenerateString.h"


namespace lee8871_support {
	class iJsonTransformer {
	private:
		int quote_counter = 1;
	protected:
		iJsonTransformer() {	}
		virtual~iJsonTransformer() {	}
	public:
		iJsonTransformer*  quote() {
			quote_counter++;
			return this;
		}
		void freeQuote() {
			if(--quote_counter == 0){
				delete this;
			}
		}
		virtual int get(JsonGenerateString* str, const void *value) = 0;
		virtual int set(JsonParseString* str, void *value) = 0;
	};

	class Json {
	private:
		void * value_prt = nullptr;
		iJsonTransformer* _transform = nullptr;

		void moveFrom(Json&);
		void copyFrom(const Json&);
		friend class asArray;
		friend class asObject;
		friend class JsonPtr;
		Json operator = (const Json&);
	public:
		inline iJsonTransformer* Transform() {
			return _transform;
		}
		Json(Json&&);
		Json(const Json&);
		void cloneTransport(const Json&, void*);

		Json();

		Json(iJsonTransformer* _transform, void* value_prt);
		Json(srb::int32* value_prt);
		Json(srb::uint16* value_prt);
		Json(srb::uint8* value_prt);
		Json(srb::int8 * value_prt);

		Json(std::initializer_list<Json> v);//to json array
		Json(std::initializer_list<std::pair<const char*, Json>> v);//to json object

		~Json();
		int get(JsonGenerateString* str, const void *diff = 0);
		int set(JsonParseString* str, void *diff = 0);
	};

	Json buildJsonConstStr(const char * value_prt);
	Json buildJsonStr(char * value_prt, int max_size);
	Json buildJsonPtr(const Json& json, void* Value_pp);
	Json buildUint8Str(uint8 * value_prt, int max_size);
	Json buildUint8Array(uint8 * value_prt, int max_size);





	class JSON_INITIELAZATION_CLASS {
	public:
		JSON_INITIELAZATION_CLASS();
		~JSON_INITIELAZATION_CLASS();
	};
	static JSON_INITIELAZATION_CLASS __JSON_INITIELAZATION_OBJECT;
};
