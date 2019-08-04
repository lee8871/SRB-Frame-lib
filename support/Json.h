#pragma once
#include "lee.h"
#include <initializer_list>
#include <vector>

using namespace std;
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
		void * value_prt;
		eJsonType type;
		const char * name;
		unsigned int hash;
		int len;
		const json* && _test_pointJson;
		
		json(const json&) = delete;
		json(const char* name, int* value_prt);
		json(int* value_prt);
		json(const char* name, json* value_prt, int len = 1, bool is_array = false);
		json(json* value_prt, int len = 1, bool is_array = false);
		json(const char* name, initializer_list<json> v, bool is_array = false);
		int getJson(char * buf, int buf_size);
	};

	class jsonArray {
		const json* objects;
	public:
		jsonArray(initializer_list<json> v) {
			objects = v.begin();
		}
	};


	





};