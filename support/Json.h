#pragma once
#include "lee.h"
#include <initializer_list>

using namespace std;





namespace atjson {
	enum class eJsonType;
	class StringPtr {
	private:
		char* buf;
		size_t ptr_i = 0;
		size_t size;
	public:
		StringPtr(char* b, int s) :
			buf(b), size(s) {
			buf[size - 1] = 0;
		}

		inline char* ptr() {
			return buf + ptr_i;
		}
		inline int inc(int increase) {
			if (increase < 0) {
				return -100 + increase;
			}
			ptr_i += increase;
			if (ptr_i == size) {
				return srb::fail;
			}
			return srb::done;
		}
		inline size_t rem() {
			return (size - ptr_i);
		}
		int append(char* append);
		int append(char append);
		int printf(const char *format, ...);

	};





	using transformCB = int(*)(StringPtr*, void*, int);

	int writeNull(StringPtr*,void*,int);

	class json{
	private:
		int get(StringPtr* str);
		int getJsonKey(StringPtr* str);
		void copyValueFrom(const json&);
		friend int writeArray(StringPtr* str, void* value_prt, int size);
		friend int writeObject(StringPtr* str, void* value_prt, int size);
		void * value_prt = nullptr;
		int size = 0;
		transformCB tran = writeNull;
		const char * name = nullptr;
		unsigned int hash = 0;
		
		void moveFrom(json&);
		void copyFrom(const json&);

	public:

		json operator = (const json&);
		json(json&&);
		json(const json&);

		json();
		json(int* value_prt);

		json(initializer_list<json> v);//to json array
		json(initializer_list<pair<const char*, json>> v);//to json object
		~json();
		int getJson(char * buf, int buf_size);
	};







};