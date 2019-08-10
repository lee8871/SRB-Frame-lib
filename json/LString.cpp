#include <cstdarg>
#include <cstdio>

#include "LString.h"


namespace lee8871_support {
#define addAndCheck(a) *(_ptr++)= (a);if (_ptr == _end) { *(_ptr - 1) = 0;return buf_use_up; }
#define endString() *_ptr = 0; return done;
	LString::LString(char *buf, int size) :
		_end(buf + size), _buf(buf), _ptr(buf) {}

	LString::LString(int size){
		_buf = _owned_buf = new char[size];
		if (_owned_buf == nullptr) {
			_ptr = _end = _buf;
		}
		else{
			_end = _buf + size;
			_ptr = _buf;
		}
	}

	LString::~LString(){
		if (_owned_buf != nullptr) {
			delete[] _owned_buf;
		}
	}

	int LString::append(char a) {
		addAndCheck(a);
		endString();
	}
	void LString::reset() {
		_ptr = _buf;
	}

	void LString::clear()	{
		_ptr = _buf;
		*_ptr = 0;
	}

	int LString::append(const char* a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::append(const char * a, int length){
		for(int i =0;((i<length)&&(*a!=0));i++) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int increase = vsnprintf(_ptr, _end - _ptr, format, args);
		va_end(args);
		if (increase < 0) {
			return -210 + increase;
		}
		_ptr += increase;
		if (_ptr == _end) {
			return buf_use_up;
		}
		return done;
	}
};