#pragma once
#include "lee.h"
#include <string>


namespace lee8871_support {

	class LString {
	private:
		char* _owned_buf=nullptr;
		char* _buf = nullptr;
		char* _end = nullptr;
	protected:
		char* ptr = nullptr;
	public:
		char *const& Buf = _buf;
		char *const& End = _end;
		LString(char *, int);
		LString(int);
		~LString();
		void reset();
		void clear();
		int append(const char* a);
		int append(char append);
		int print(const char *format, ...);
	};
};