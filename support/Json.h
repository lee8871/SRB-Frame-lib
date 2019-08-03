#pragma once
#include "lee.h"
#include <initializer_list>

namespace atjson {
	enum class eJsonType;
	class StringPtr ;
	class json{
	private:
		int get(StringPtr* str);
		int getJsonKey(StringPtr* str);
		int writeValue(StringPtr* str);

		int set(const char* buf);

	public:
		const char * name;
		unsigned int hash;
		void * value_prt;
		int len;
		eJsonType type;
		json(const json&) = delete;
		json(const char* name, int* value_prt);
		json(int* value_prt);
		json(const char* name, json* value_prt, int len = 1,bool is_array = false);
		int getJson(char * buf, int buf_size);
	};








};