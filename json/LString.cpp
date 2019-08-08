#include <cstdarg>
#include <cstdio>

#include "LString.h"


namespace lee8871_support {
#define addAndCheck(a) *(ptr++)= (a);if (ptr == _end) { *(ptr - 1) = 0;return buf_use_up; }
#define endString() *ptr = 0; return done;
	LString::LString(char *buf, int size) :
		_end(buf + size), _buf(buf), ptr(buf) {}

	LString::LString(int size){
		_buf = _owned_buf = new char[size];
		if (_owned_buf == nullptr) {
			ptr = _end = _buf;
		}
		else{
			_end = _buf + size;
			ptr = _buf;
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
		ptr = _buf;
	}

	void LString::clear()	{
		ptr = _buf;
		*ptr = 0;
	}

	int LString::append(const char* a) {
		while (*a != 0) {
			addAndCheck(*a);
			a++;
		}
		endString();
	}
	int LString::print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int increase = vsnprintf(ptr, _end - ptr, format, args);
		va_end(args);
		if (increase < 0) {
			return -210 + increase;
		}
		ptr += increase;
		if (ptr == _end) {
			return buf_use_up;
		}
		return done;
	}
};