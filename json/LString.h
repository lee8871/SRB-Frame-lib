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
		char* _ptr = nullptr;
	public:
		char *const& Buf = _buf;
		char *const& End = _end;
		char *const& Ptr = _ptr;
		LString(char *, int);
		LString(int);
		~LString();
		void reset();
		void clear();
		int append(const char* a);
		int append(const char* a,int length);
		int append(char append);
		int print(const char *format, ...);
		int checkOverflow();
		
		int foward();
		int jump(int inc);
		char nextChar();
		bool checkCh(char c);
		void outputRemoveSpace();

		bool checkStr(char* check) {
			outputRemoveSpace();
			char* p = _ptr;
			while (*p == *check) {
				p++;check++;
				if (check == 0) {
					_ptr = p;
					return true;
				}
			}
			return false;
		}
	};
};