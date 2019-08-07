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

	class json{
	private:


		friend int asArray(transformCBArgumenrt);
		friend int asObject(transformCBArgumenrt);
		int getArray(JsonString* str, void *diff);
		int getObject(JsonString* str, void *diff);
		int setArray(JsonString* str, void *diff);
		int setObject(JsonString* str, void *diff);

		const char * name = nullptr;
		unsigned int hash = 0;


		void copyValueFrom(const json&);
		void moveFrom(json&);
		void copyFrom(const json&);


	public:
		void * value_prt = nullptr;
		int size = 0;
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



};