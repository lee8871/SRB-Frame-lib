#pragma once
#include "lee.h"
#include <string>


namespace lee8871_support {

	class LString {
	private:
		char* _owned_buf=nullptr;
		char* _buf = nullptr;
		char* _end = nullptr;
		char* _ptr = nullptr;
	protected:
	public:
		char *const& Buf = _buf;
		char *const& End = _end;
		char *const& Ptr = _ptr;
		LString(char *, int);
		LString(int);
		LString(const char * path);
		~LString();
		inline void reset() {
			_ptr = _buf;
		}
		inline void clear() {
			_ptr = _buf;
			*_ptr = 0;
		}
		int append(const char* a);
		int append(const char* a,int length);
		int append(char append);
		int print(const char *format, ...);

		inline int remainder(){
			return _end - _ptr;
		}
		int checkOverflow();
		


		int forward();
		int forward(int inc);
		bool checkCh(char c);
		bool checkStr(const char* check);






		int writeToFile(const char * path);
		

		inline bool isSpace() {
			return (*_ptr <= ' ') ;
		}


		inline bool isStrEnd() {
			return ((_ptr == End - 1) || (*_ptr == 0));
		}



		int nextGroup() {
			while (1) {
				if (isStrEnd()) {
					return fail;
				}
				if (isSpace()) {
					removeSpace();
					return done;
				}
				_ptr++;
			}
		}

		int removeSpace() {
			while (_ptr != End) {
				if (isStrEnd()) {
					return fail;
				}
				if (!isSpace()) {
					return done;
				}
				_ptr++;
			}
		}

		int getGroup(char* buf_to, int len_to) {
			char * temp_ptr = _ptr;
			for (int i = 0;i < len_to;i++) {
				if (isStrEnd()) {
					buf_to[i] = 0;
					return i;
				}
				if (isSpace()) {
					buf_to[i] = 0;
					removeSpace();
					return i;
				}
				buf_to[i] = *_ptr;
				_ptr++;				
			}
			buf_to[len_to - 1] = 0;
			_ptr = temp_ptr;
			return fail;
		}	

	};
};