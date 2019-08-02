#pragma once
#include "lee.h"
#include <initializer_list>

namespace atjson {
	enum class eJsonType;
	class StringPtr ;
	class json{
	private:
		int get(StringPtr* str);
		int get_obj(StringPtr* str);
		int get_i32(StringPtr* str);


		int set(const char* buf);

	public:
		const char * name;
		unsigned int hash;
		void * value_prt;
		int len;
		eJsonType type;
		json(const json&) = delete;
		json(const char* name, int* value_prt, int len=1);
		json(const char* name, json* json, int len = 1);
		friend int getJson(char* buf, int buf_size, json* objectt, int len);
	};








};