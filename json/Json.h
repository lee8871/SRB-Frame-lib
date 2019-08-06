#pragma once
#include "lee.h"
#include "lstring.h"
#include <initializer_list>




namespace lee8871_support {
	enum class eJsonError {
		
	};


	using transformCB = int(*)(LString*, void*,bool);

	int asNull(LString*,void*, bool);

	class json{
	private:
		int getJsonKey(LString* str);
		int getArray(LString* str, void *diff);
		int getObject(LString* str, void *diff);

		void * value_prt = nullptr;

		int size = 0;
		transformCB transform = asNull;
		const char * name = nullptr;
		unsigned int hash = 0;

		void copyValueFrom(const json&);
		void moveFrom(json&);
		void copyFrom(const json&);
	public:
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
		int get(LString* str, void *diff = 0);
	};



	json jsonString(const char * value_prt);





};